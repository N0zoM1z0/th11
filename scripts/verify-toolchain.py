#!/usr/bin/env python3
"""Attest and execute the pinned TH11 VC9 RTM toolchain headlessly."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import tomllib

from linked_image import linked_functions, verify_capstone


ROOT = Path(__file__).resolve().parents[1]
LOCK = ROOT / "config" / "tools.lock.toml"
REQUIRED_COMPONENTS = {
    "compiler", "c_frontend", "cpp_frontend", "optimizer", "linker",
    "pdb_backend", "pdb_core", "pdb_server", "resource_compiler",
    "resource_runtime", "resource_converter", "object_runtime",
}


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def load_lock() -> tuple[dict[str, object], dict[str, object], dict[str, object]]:
    with LOCK.open("rb") as stream:
        manifest = tomllib.load(stream)
    tool = manifest["msvc90"]
    wine = manifest["wine"]
    decoder = manifest["capstone"]
    if tool.get("identity_state") != "observed-candidate":
        raise ValueError("VC9 lock must remain an observed candidate")
    if wine.get("architecture") != "win32" or wine.get("display_mode") != "xvfb-headless":
        raise ValueError("Wine lock must require a headless win32 prefix")
    if decoder.get("authority") != "linked-image-field-decoder":
        raise ValueError("Capstone lock has an invalid authority")
    components = tool.get("components", {})
    if not REQUIRED_COMPONENTS.issubset(components):
        raise ValueError("VC9 component lock is incomplete")
    for name, component in components.items():
        if not isinstance(component, dict) or not component.get("path"):
            raise ValueError(f"invalid VC9 component declaration: {name}")
        digest = str(component.get("sha256", ""))
        if len(digest) != 64 or any(char not in "0123456789abcdef" for char in digest):
            raise ValueError(f"invalid VC9 component SHA-256: {name}")
    if len(bytes.fromhex(str(tool["ltcg_object_header"]))) != 8:
        raise ValueError("invalid observed VC9 LTCG header")
    if int(tool["linked_smoke_extent_size"]) <= 0:
        raise ValueError("invalid observed VC9 smoke extent")
    for script in (
        "scripts/repo-python",
        "scripts/run-headless-wine.sh",
        "scripts/compile-probe.sh",
    ):
        if not os.access(ROOT / script, os.X_OK):
            raise ValueError(f"toolchain wrapper is not executable: {script}")
    for probe in (
        "probes/toolchain-smoke.c",
        "probes/toolchain-smoke.cpp",
        "probes/toolchain-smoke.rc",
    ):
        if not (ROOT / probe).is_file():
            raise ValueError(f"toolchain smoke input is missing: {probe}")
    return tool, wine, decoder


def tool_root(tool: dict[str, object]) -> Path:
    selected = os.environ.get("TH11_MSVC90_ROOT", str(ROOT / str(tool["selection"])))
    return Path(selected).expanduser().resolve(strict=True)


def prefix_root(wine: dict[str, object]) -> Path:
    selected = os.environ.get(
        "TH11_WINEPREFIX", os.environ.get("WINEPREFIX", str(ROOT / str(wine["prefix"])))
    )
    return Path(selected).expanduser().absolute()


def run(
    command: list[str],
    *,
    environment: dict[str, str] | None = None,
    check: bool = True,
) -> subprocess.CompletedProcess[str]:
    scratch = ROOT / ".tools" / "tmp"
    scratch.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryFile(mode="w+", encoding="utf-8", errors="replace", dir=scratch) as output:
        completed = subprocess.run(
            command, cwd=ROOT, env=environment, check=False,
            stdout=output, stderr=subprocess.STDOUT,
        )
        output.seek(0)
        result = subprocess.CompletedProcess(command, completed.returncode, stdout=output.read())
    if check and result.returncode:
        raise subprocess.CalledProcessError(result.returncode, command, output=result.stdout)
    return result


def wine_environment(prefix: Path) -> dict[str, str]:
    environment = os.environ.copy()
    environment.update(
        {
            "WINEARCH": "win32",
            "WINEPREFIX": str(prefix),
            "TH11_WINEPREFIX": str(prefix),
            "WINEDEBUG": "-all",
            "WINEDLLOVERRIDES": "mscoree,mshtml=",
        }
    )
    return environment


def headless(
    environment: dict[str, str], *command: str, check: bool = True
) -> subprocess.CompletedProcess[str]:
    return run(
        [str(ROOT / "scripts" / "run-headless-wine.sh"), *command],
        environment=environment,
        check=check,
    )


def windows_path(environment: dict[str, str], path: Path) -> str:
    return run(["winepath", "-w", str(path.resolve())], environment=environment).stdout.strip().replace("\r", "")


def verify_pe32(path: Path) -> None:
    data = path.read_bytes()
    if data[:2] != b"MZ" or len(data) < 0x40:
        raise ValueError("headless link smoke did not produce an MZ image")
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe : pe + 4] != b"PE\0\0" or struct.unpack_from("<H", data, pe + 4)[0] != 0x14C:
        raise ValueError("headless link smoke did not produce PE32 i386")


def verify_components(tool: dict[str, object]) -> dict[str, object]:
    selected = tool_root(tool)
    observed = {}
    for component, declaration in sorted(tool["components"].items()):
        relative = str(declaration["path"])
        path = (selected / relative).resolve()
        path.relative_to(selected)
        if not path.is_file():
            raise ValueError(f"missing toolchain component: {relative}")
        actual = sha256(path)
        if actual != declaration["sha256"]:
            raise ValueError(f"toolchain hash mismatch for {relative}: {actual}")
        observed[component] = {"sha256": actual, "size": path.stat().st_size}
    return observed


def execute(
    tool: dict[str, object],
    wine: dict[str, object],
    decoder: dict[str, object],
) -> dict[str, object]:
    selected = tool_root(tool)
    prefix = prefix_root(wine)
    environment = wine_environment(prefix)
    wine_binary = os.environ.get("WINE", "wine")
    if shutil.which(wine_binary) is None or shutil.which("winepath") is None:
        raise ValueError("system Wine and winepath are required")
    if shutil.which("xvfb-run") is None:
        raise ValueError("xvfb-run is required for headless Wine")

    decoder_identity = verify_capstone(decoder)

    observed = verify_components(tool)
    run([sys.executable, str(ROOT / "scripts/bootstrap-ntlm-auth.py"), "--check"])
    environment["WINEPATH"] = windows_path(environment, selected / "VC/bin")
    for required in ("VC/include", "VC/lib", "Microsoft SDKs/Windows/v6.0A/Include", "Microsoft SDKs/Windows/v6.0A/Lib"):
        if not (selected / required).is_dir():
            raise ValueError(f"missing toolchain directory: {required}")

    if not (prefix / "system.reg").is_file():
        headless(environment, "wineboot", "-u")
    compiler_output = headless(environment, wine_binary, str(selected / "VC/bin/cl.exe")).stdout
    linker_output = headless(
        environment,
        wine_binary,
        str(selected / "VC/bin/link.exe"),
        check=False,
    ).stdout
    if str(tool["compiler_banner"]) not in compiler_output:
        raise ValueError("compiler banner does not match the lock")
    if str(tool["linker_banner"]) not in linker_output:
        raise ValueError("linker banner does not match the lock")

    build_root = ROOT / "build"
    build_root.mkdir(exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="toolchain-smoke-", dir=build_root) as raw:
        scratch = Path(raw)
        c_obj = scratch / "normal-c.obj"
        cpp_obj = scratch / "normal-cpp.obj"
        ltcg_obj = scratch / "ltcg-cpp.obj"
        resource = scratch / "probe.res"
        executable = scratch / "probe.exe"
        map_path = scratch / "probe.map"
        linked_pdb = scratch / "probe.pdb"
        compile_script = ROOT / "scripts" / "compile-probe.sh"
        for source, output, flags in (
            (ROOT / "probes/toolchain-smoke.c", c_obj, ["/O2", "/Gy", "/GS-"]),
            (ROOT / "probes/toolchain-smoke.cpp", cpp_obj, ["/O2", "/Gy", "/GS-"]),
            (ROOT / "probes/toolchain-smoke.cpp", ltcg_obj, ["/O2", "/GL", "/Gy", "/GS-"]),
        ):
            run([str(compile_script), str(source), str(output), *flags], environment=environment)
        if c_obj.read_bytes()[:2] != b"L\x01" or cpp_obj.read_bytes()[:2] != b"L\x01":
            raise ValueError("normal compiler smoke did not produce i386 COFF")
        if ltcg_obj.read_bytes()[:8] != bytes.fromhex(str(tool["ltcg_object_header"])):
            raise ValueError("/GL compiler smoke did not produce an LTCG object")

        rc = selected / "Microsoft SDKs/Windows/v6.0A/Bin/RC.Exe"
        rc_args = [
            wine_binary,
            str(rc),
            "/fo" + windows_path(environment, resource),
            windows_path(environment, ROOT / "probes/toolchain-smoke.rc"),
        ]
        rc_output = headless(environment, *rc_args).stdout
        if str(tool["resource_compiler_banner"]) not in rc_output:
            raise ValueError("resource compiler banner does not match the lock")
        if resource.read_bytes()[:4] != b"\x00\x00\x00\x00":
            raise ValueError("resource compiler smoke did not produce an MSVC resource")

        link = selected / "VC/bin/link.exe"
        headless(
            environment,
            wine_binary,
            str(link),
            "/nologo",
            "/ltcg",
            "/nodefaultlib",
            "/subsystem:console",
            "/entry:factory_toolchain_cpp_probe",
            "/incremental:no",
            "/fixed:no",
            "/debug",
            "/debugtype:cv",
            "/opt:noref",
            "/opt:noicf",
            "/map:" + windows_path(environment, map_path),
            "/pdb:" + windows_path(environment, linked_pdb),
            "/out:" + windows_path(environment, executable),
            windows_path(environment, ltcg_obj),
            windows_path(environment, resource),
        )
        verify_pe32(executable)
        linked = linked_functions(executable, map_path, linked_pdb)
        smoke_functions = [
            function
            for function in linked["functions"]
            if function["symbol"] == "_factory_toolchain_cpp_probe"
        ]
        if len(smoke_functions) != 1 or smoke_functions[0]["size"] != int(tool["linked_smoke_extent_size"]):
            raise ValueError(
                "linked-image extent smoke did not recover the expected LTCG function"
            )

    return {
        "ready": True,
        "identity_pass": True,
        "execution_pass": True,
        "linked_image_extent_pass": True,
        "linked_image_decoder_pass": True,
        "ntlm_auth_pass": True,
        "capstone_version": decoder_identity["version"],
        "compiler_banner": str(tool["compiler_banner"]),
        "linker_banner": str(tool["linker_banner"]),
        "toolchain_commit": str(tool["commit"]),
        "wine_architecture": "win32",
        "display_mode": "xvfb-headless",
        "components": observed,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    action = parser.add_mutually_exclusive_group(required=True)
    action.add_argument("--check-declarations", action="store_true", help="validate tracked declarations without private tools")
    action.add_argument("--check", action="store_true", help="hash-attest private compiler components and the pinned decoder")
    action.add_argument("--execute", action="store_true", help="run the real hash, banner, compile, resource, and link probes")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        tool, wine, decoder = load_lock()
        if args.execute:
            report = execute(tool, wine, decoder)
        elif args.check:
            report = {
                "ready": False,
                "identity_pass": True,
                "components": verify_components(tool),
                "decoder": verify_capstone(decoder),
                "execution_state": "not-requested",
            }
        else:
            report = {"ready": False, "declarations_pass": True, "execution_state": "not-requested"}
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError, tomllib.TOMLDecodeError) as error:
        report = {"ready": False, "error": str(error)}
        if isinstance(error, subprocess.CalledProcessError) and error.stdout:
            report["command_output"] = error.stdout[-4000:]
    if args.json:
        print(json.dumps(report, indent=2, sort_keys=True))
    elif report.get("ready"):
        print("TH11 toolchain OK: VC9 RTM normal COFF + LTCG + resource + PE32 link (headless Wine)")
    elif report.get("identity_pass"):
        print("TH11 toolchain identity OK; execution not requested")
    elif report.get("declarations_pass"):
        print("TH11 toolchain declarations OK; execution not requested")
    else:
        print(f"error: TH11 toolchain verification failed: {report.get('error')}", file=sys.stderr)
    return 0 if report.get("ready") or report.get("identity_pass") or report.get("declarations_pass") else 1


if __name__ == "__main__":
    raise SystemExit(main())

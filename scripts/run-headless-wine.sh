#!/usr/bin/env bash
set -euo pipefail

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
wine_prefix=${TH11_WINEPREFIX:-${WINEPREFIX:-$repo_root/.tools/wineprefix-msvc900}}

if [[ $# -lt 1 ]]; then
  echo "usage: $0 COMMAND [ARG...]" >&2
  exit 2
fi
if ! command -v xvfb-run >/dev/null 2>&1; then
  echo "missing xvfb-run required for headless Wine" >&2
  exit 1
fi

wine_tmpdir="$repo_root/.tools/tmp"
mkdir -p "$wine_prefix" "$wine_tmpdir"
exec flock --close "$repo_root/.tools/heavy-tool.lock" nice -n 10 env \
  PATH="$repo_root/scripts:$PATH" \
  TMPDIR="$wine_tmpdir" \
  WINEARCH=win32 \
  WINEPREFIX="$wine_prefix" \
  WINEDEBUG="${TH11_WINEDEBUG:--all}" \
  WINEDLLOVERRIDES=mscoree,mshtml= \
  xvfb-run -a -s '-screen 0 1024x768x24 -nolisten tcp' "$@"

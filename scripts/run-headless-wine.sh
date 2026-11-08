#!/usr/bin/env bash
# Resolve Wine through Factory while retaining repository-owned state.
factory_repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
export PATH="$factory_repo_root/.tools/factory-bin:$PATH"
set -euo pipefail

repo_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
wine_prefix=${TH11_WINEPREFIX:-$repo_root/.tools/wineprefix-msvc900}

if [[ $# -lt 1 ]]; then
  echo "usage: $0 COMMAND [ARG...]" >&2
  exit 2
fi
mkdir -p "$wine_prefix"
wine_cpu=$(python3 -c 'import os; print(min(os.sched_getaffinity(0)))')
# This Wine build aborts in the Factory sandbox when TMPDIR is set. Use its
# default temporary directory; the selected prefix and compiler lock persist.
exec flock --close "$repo_root/.tools/heavy-tool.lock" nice -n 10 taskset --cpu-list "$wine_cpu" env -u TMPDIR \
  PATH="$repo_root/scripts:$PATH" \
  WINEARCH=win32 \
  WINEPREFIX="$wine_prefix" \
  WINEDEBUG="${TH11_WINEDEBUG:--all}" \
  DISPLAY= WAYLAND_DISPLAY= \
  WINEDLLOVERRIDES='winemenubuilder.exe=d;mscoree,mshtml=d' \
  "$@"

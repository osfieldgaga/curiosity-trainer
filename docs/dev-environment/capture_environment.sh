#!/usr/bin/env bash
# Captures the development environment used for the Curiosity Trainer project into
# environment.json and vscode-extensions.txt (next to this script).
# Run it again after updating VS Code, the MPLAB extensions, XC16 or the packs.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
PACKS="${MCHP_PACKS:-$HOME/.mchp_packs}"
APPS="$HOME/.mplab/app-finder/apps"

code --list-extensions --show-versions 2>/dev/null \
  | grep -i -E '^(microchip\.|ms-vscode\.(cpptools|cmake-tools)|ms-python\.|mathworks\.)' \
  > "$HERE/vscode-extensions.txt"

python3 - "$HERE/environment.json" "$PACKS" "$APPS" <<'PY'
import json, os, platform, subprocess, sys, datetime, glob
out, packs, apps = sys.argv[1:4]
def run(cmd):
    try: return subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=30).stdout.strip()
    except Exception: return ""
def vkey(v): return [int(n) if n.isdigit() else 0 for n in v.lstrip("v").split(".")]
def versions(d): return sorted(os.listdir(d), key=vkey) if os.path.isdir(d) else []  # oldest to newest
os_name = run(". /etc/os-release 2>/dev/null && echo $PRETTY_NAME") or platform.platform()
env = {
  "captured": datetime.date.today().isoformat(),
  "os": {"name": os_name, "kernel": platform.release()},
  "vscode": {"version": run("code --version | head -1"),
             "extensions": [l for l in run("cat '%s'" % out.replace("environment.json","vscode-extensions.txt")).splitlines() if l]},
  "compilers": {"xc16": versions("/opt/microchip/xc16"), "xc32": versions("/opt/microchip/xc32"),
                "xc16_in_use": run("/opt/microchip/xc16/v2.10/bin/xc16-gcc --version 2>&1 | head -1")},
  "mplab_x_ide": versions("/opt/microchip/mplabx"),
  "packs": {os.path.basename(p): versions(p) for p in glob.glob(os.path.join(packs, "Microchip", "*"))},
  "mplab_extension_tools": {os.path.basename(p): versions(p) for p in glob.glob(os.path.join(apps, "*"))},
  "python": {"version": platform.python_version(),
             "packages": run("python3 -c \"import serial,numpy,matplotlib;print('pyserial '+serial.__version__+', numpy '+numpy.__version__+', matplotlib '+matplotlib.__version__)\"")},
  "matlab": versions("/usr/local/MATLAB"),
}
json.dump(env, open(out, "w"), indent=2)
print("wrote", out)
PY

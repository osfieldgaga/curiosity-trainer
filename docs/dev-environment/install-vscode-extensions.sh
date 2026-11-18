#!/usr/bin/env bash
# Installs the exact VS Code extension versions listed in vscode-extensions.txt.
# Usage: ./install-vscode-extensions.sh            (all extensions)
#        ./install-vscode-extensions.sh --latest   (same extensions, latest versions)
HERE="$(cd "$(dirname "$0")" && pwd)"
while read -r ext; do
  [ -z "$ext" ] && continue
  [ "${1:-}" = "--latest" ] && ext="${ext%@*}"
  echo "Installing $ext"
  code --install-extension "$ext" --force
done < "$HERE/vscode-extensions.txt"

# Installs the exact VS Code extension versions listed in vscode-extensions.txt (Windows).
# Usage: .\install-vscode-extensions.ps1            (all extensions)
#        .\install-vscode-extensions.ps1 -Latest    (same extensions, latest versions)
param([switch]$Latest)
Get-Content "$PSScriptRoot\vscode-extensions.txt" | Where-Object { $_ -ne "" } | ForEach-Object {
    $ext = if ($Latest) { $_.Split("@")[0] } else { $_ }
    Write-Host "Installing $ext"
    code --install-extension $ext --force
}

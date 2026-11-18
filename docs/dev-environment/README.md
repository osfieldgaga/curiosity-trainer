# Development environment snapshot

Reference configuration used to develop the Curiosity Trainer firmware with MPLAB in VS Code.
The full installation guide is `../Installing MPLAB in VS Code.docx`.

| File | Content |
|---|---|
| `environment.json` | Versions of the OS, VS Code, XC16, MPLAB X, packs, MPLAB tools, Python and MATLAB |
| `vscode-extensions.txt` | VS Code extensions with their exact versions |
| `install-vscode-extensions.sh` / `.ps1` | Install those extensions (Linux/macOS / Windows), `--latest` / `-Latest` for the newest versions |
| `capture_environment.sh` | Regenerates `environment.json` and `vscode-extensions.txt` from the current machine |
| `vscode/` | The project's `.vscode` configuration (MPLAB project, launch, settings), made portable |
| `linux/z010_mchp_tools.rules` | Microchip udev rule giving users access to the debugger (Linux only) |

To restore the VS Code configuration, copy the files of `vscode/` into the project's `.vscode` folder
(rename `CuriosityTrainer.mplab-local.example.json` to `CuriosityTrainer.mplab-local.json`, or let the
extension create it when you select the board).

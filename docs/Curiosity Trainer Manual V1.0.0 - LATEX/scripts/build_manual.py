"""Assemble the unified KOMA-Script manual: Part I (VS Code installation) + Part II
(programming reference, converted by docx2tex.py).
Usage: python3 build_manual.py <part2.tex> <dev-environment dir> <logo.png> <out dir>"""
import json, os, re, shutil, sys

PART2, ENVDIR, LOGO, OUTDIR = sys.argv[1:5]
env = json.load(open(os.path.join(ENVDIR, 'environment.json')))
exts = [l.strip() for l in open(os.path.join(ENVDIR, 'vscode-extensions.txt')) if l.strip()]

# ---------- helpers ----------
UNI = {'≈': r'$\approx$', '±': r'$\pm$', 'µ': r'\textmu{}', '−': r'$-$', '×': r'$\times$', '→': r'$\rightarrow$',
       '’': "'", '“': '``', '”': "''", '–': '--', '—': '---', '\u00a0': '~', '…': r'\dots{}'}
def esc(s):
    out = []
    for k, ch in enumerate(s):
        if ch == '-' and k + 1 < len(s) and s[k + 1] == '-': out.append('-{}'); continue
        if ch == '\\': out.append(r'\textbackslash{}')
        elif ch in '&%$#_{}': out.append('\\' + ch)
        elif ch == '~': out.append(r'\textasciitilde{}')
        elif ch == '^': out.append(r'\textasciicircum{}')
        elif ch in UNI: out.append(UNI[ch])
        else: out.append(ch)
    return ''.join(out)
def tt(s):  # monospace, with line break points after _ / . - (typewriter fonts do not hyphenate)
    t = esc(s).replace(r'\_', r'\_\allowbreak{}')
    t = re.sub(r'([/.-])(?!\{\})', lambda m: m.group(1) + r'\allowbreak{}', t)
    return r'\texttt{' + t + '}'
def md(s):
    """`code`, **bold** and {ref:label} markup to LaTeX."""
    out, i = [], 0
    for m in re.finditer(r'`([^`]+)`|\*\*([^*]+)\*\*|\{ref:([a-z0-9-]+)\}', s):
        out.append(esc(s[i:m.start()]))
        if m.group(1) is not None: out.append(tt(m.group(1)))
        elif m.group(2) is not None: out.append(r'\textbf{' + esc(m.group(2)) + '}')
        else: out.append(r'\ref{' + m.group(3) + '}')
        i = m.end()
    out.append(esc(s[i:]))
    return ''.join(out)
def vnum(v): return [int(n) if n.isdigit() else 0 for n in v.lstrip('v').split('.')]
def newest(a): return sorted(a, key=vnum)[-1] if a else ''

L = []
def P(s): L.append('\n' + md(s))
def chapter(t, label): L.append('\n' + r'\chapter{' + esc(t) + r'}\label{' + label + '}')
def section(t, label=None): L.append('\n' + r'\section{' + esc(t) + '}' + (r'\label{' + label + '}' if label else ''))
def subsection(t): L.append('\n' + r'\subsection{' + esc(t) + '}')
def items(lst, env='itemize'):
    L.append(r'\begin{' + env + '}'); L.extend(r'\item ' + md(x) for x in lst); L.append(r'\end{' + env + '}')
def steps(lst): items(lst, 'enumerate')
def code(lines, lang='shell'):
    L.append(r'\begin{lstlisting}[style=' + lang + ']'); L.extend(lines); L.append(r'\end{lstlisting}')
def note(label, text): L.append(r'\begin{notebox}\textbf{' + esc(label) + '} ' + md(text) + r'\end{notebox}')
def longtable(header, rows, spec):
    L.append(r'\begin{longtable}{' + spec + '}')
    L.append(r'\toprule')
    head = ' & '.join(r'\textbf{' + esc(h) + '}' for h in header) + r' \\'
    L.extend([head, r'\midrule', r'\endfirsthead', r'\toprule', head, r'\midrule', r'\endhead', r'\bottomrule', r'\endlastfoot'])
    L.extend(' & '.join(md(c) for c in r) + r' \\' for r in rows)
    L.append(r'\end{longtable}')

def col(w): return r'>{\raggedright\arraybackslash}p{' + w + r'\linewidth}'

# ---------- Part I data ----------
ext_role = {
    'microchip.mplab-extension-pack': 'Installs the whole MPLAB set below in one step',
    'microchip.mplab-extensions-core': 'Projects, build, the MPLAB view',
    'microchip.mplab-extensions-platforms': 'Downloads and runs the MPLAB tools (backend, make, ninja, Java)',
    'microchip.toolchains': 'Finds the XC compilers (XC16 for this project)',
    'microchip.mplab-core-da': 'Debugger and programmer (the mplab-core-da launch type)',
    'microchip.mplab-ui': 'MPLAB user interface: project properties, packs, tools',
    'microchip.mplabx-importer': 'Imports an MPLAB X project (.X folder) into VS Code',
    'microchip.mplab-clangd': 'Code completion and navigation for XC16 code',
    'microchip.runcmake': 'Generates the CMake/ninja build of the project',
    'microchip.mplab-code-configurator': 'MCC code generator (not used by this project)',
    'microchip.mplab-data-visualizer': "Serial terminal and plots for the debugger's COM port",
    'microchip.mplab-kconfig': 'Kconfig support (not used by this project)',
    'microchip.mplab-ai-coding-assistant': 'Microchip AI assistant (optional)',
    'ms-vscode.cpptools': 'C/C++ language support',
    'ms-vscode.cpptools-extension-pack': 'C/C++ extension pack',
    'ms-vscode.cpptools-themes': 'C/C++ color themes',
    'ms-vscode.cmake-tools': 'CMake support, used by the MPLAB build',
    'ms-python.python': 'Python, for tools/mic_viewer.py (optional)',
    'ms-python.vscode-pylance': 'Python language server (optional)',
    'ms-python.debugpy': 'Python debugger (optional)',
    'ms-python.vscode-python-envs': 'Python environments (optional)',
    'mathworks.language-matlab': 'MATLAB, for tools/mic_viewer.m (optional)',
}
def ext_order(e):
    i = e.split('@')[0]
    return (0 if i == 'microchip.mplab-extension-pack' else 1 if i.startswith('microchip.') else 2 if i.startswith('ms-vscode.') else 3 if i.startswith('ms-python.') else 4, i)
exts.sort(key=ext_order)
pack_ver = next((e.split('@')[1] for e in exts if e.startswith('microchip.mplab-extension-pack@')), '')
tools = env.get('mplab_extension_tools', {})
dfp = sorted(env['packs'].get('dsPIC33CK-MC_DFP', []), key=vnum)

# ---------- Part I: development environment ----------
chapter('Overview', 'ch-env-overview')
P('This part explains how to set up VS Code with the Microchip MPLAB extensions to build, program and debug the Curiosity Trainer firmware (dsPIC33CK64MC105 on a Curiosity Nano board). It follows the exact configuration used to develop the project, listed in chapter {ref:ch-env-config}, so that everyone works with the same versions. Part {ref:part-reference} then documents the firmware itself.')
P('You will install, in this order:')
steps(['VS Code.', 'The XC16 compiler, v2.10.', 'The MPLAB extensions for VS Code, which download the rest of the tools on first start.',
       "On Linux only, the USB permissions for the board's debugger.",
       'Optionally, MPLAB X IDE (command line programming), Python and MATLAB (microphone viewer).'])
note('Saved configuration:', 'the folder `docs/dev-environment` of the project keeps a copy of this configuration: the exact versions (`environment.json`), the list of extensions with install scripts, the project\'s VS Code settings and the Linux USB rule. Chapter {ref:ch-env-saved} explains how to use and refresh it.')

chapter('Reference Configuration', 'ch-env-config')
P(f"These are the versions installed on the development machine (captured on {env['captured']}). Newer versions usually work, but when something does not build or does not connect, these are the versions known to work.")
longtable(['Component', 'Version', 'Notes'], [
    ['Operating system', env['os']['name'], f"Kernel {env['os']['kernel']}. The tools also exist for Windows and macOS"],
    ['VS Code', env['vscode']['version'], 'Any recent version should work'],
    ['MPLAB Extension Pack', pack_ver, 'Full list of extensions in chapter {ref:ch-env-ext}'],
    ['XC16 compiler', 'v2.10', 'Installed in `/opt/microchip/xc16/v2.10`' + ('. v2.00 is also installed but not used' if len(env['compilers']['xc16']) > 1 else '')],
    ['Device pack (DFP)', 'dsPIC33CK-MC_DFP 1.11.412', f"Used by the VS Code project. The MPLAB X project files use 1.5.126 (also installed: {', '.join(dfp)})"],
    ['Tool pack', 'nEDBG_TP ' + newest(env['packs'].get('nEDBG_TP', [])), "Firmware support for the board's on-board debugger"],
    ['MPLAB tools (downloaded by the extension)', f"make {newest(tools.get('make', []))}, ninja {newest(tools.get('ninja', []))}, hexmate {newest(tools.get('hexmate', []))}, Java {newest(tools.get('zulu-jre-21', []))}, backend {newest(tools.get('mplab_backend', []))}",
     'Installed automatically in `~/.mplab/app-finder/apps`'],
    ['MPLAB X IDE (optional)', newest(env['mplab_x_ide']), 'Command line programming (`ipecmd`) and the MPLAB X project in `nbproject/`'],
    ['Board', 'Curiosity Nano dsPIC33CK64MC105', 'On-board nEDBG debugger, listed as "PKOB nano" by the tools'],
    ['Python (optional)', env['python']['version'], env['python']['packages'] + ' (microphone viewer)'],
    ['MATLAB (optional)', newest(env['matlab']), 'With the MATLAB extension (microphone viewer)'],
], col('0.22') + col('0.30') + col('0.40'))

chapter('VS Code Extensions', 'ch-env-ext')
P('The MPLAB Extension Pack installs all the Microchip extensions at once. The Microsoft C/C++ and CMake extensions are installed with it. The Python and MATLAB extensions are only needed for the microphone viewer scripts in `tools/`.')
longtable(['Extension', 'Version', 'Role'], [['`' + e.split('@')[0] + '`', e.split('@')[1], ext_role.get(e.split('@')[0], '')] for e in exts],
          col('0.42') + col('0.12') + col('0.38'))

chapter('Installation', 'ch-env-install')
section('Install VS Code')
P('Download VS Code from code.visualstudio.com and install it (on Debian or Ubuntu, the .deb package). The `code` command must work in a terminal: the install scripts of chapter {ref:ch-env-saved} use it.')
section('Install the XC16 compiler', 'sec-env-xc16')
P('The firmware is compiled with **MPLAB XC16 v2.10**. Download it from the Microchip website (MPLAB XC16 Compiler, downloads archive for older versions) and run the installer:')
items(['**Linux:** make the installer executable and run it as root (the file name below is an example, use the name of the file you downloaded). It installs to `/opt/microchip/xc16/v2.10`.',
       '**Windows:** run the installer, the default folder is `C:\\Program Files\\Microchip\\xc16\\v2.10`.',
       'Choose the **free** license when asked: it is enough for this project (it compiles at -O0, the optimization level used by the project).',
       'Keep the default install folder: the MPLAB extension looks for the compilers there.'])
code(['chmod +x xc16-v2.10-full-install-linux64-installer.run', 'sudo ./xc16-v2.10-full-install-linux64-installer.run'])
P('Check the installation in a terminal (Linux path shown):')
code(['/opt/microchip/xc16/v2.10/bin/xc16-gcc --version', '# elf-gcc (Microchip Technology) 4.5.1 (XC16, Microchip v2.10) ...'])
section('Install the MPLAB extensions', 'sec-env-ext')
P('Either install the extension pack from VS Code, or install the exact versions of chapter {ref:ch-env-ext} with the script.')
subsection('Option A: from VS Code (latest versions)')
steps(['Open the Extensions view (Ctrl+Shift+X).', 'Search for **MPLAB Extension Pack** (publisher: Microchip Technology) and click Install.', 'Restart VS Code when asked.'])
subsection('Option B: exact versions with the script')
P('From the project folder, run the script for your system. Add `--latest` (Linux) or `-Latest` (Windows) to install the same extensions in their newest versions.')
code(['# Linux / macOS', './docs/dev-environment/install-vscode-extensions.sh', '', '# Windows (PowerShell)', '.\\docs\\dev-environment\\install-vscode-extensions.ps1'])
subsection('First start')
P('The first time VS Code starts with the extensions, they download their own tools: the MPLAB backend, make, ninja, hexmate and a Java runtime (Linux: `~/.mplab/app-finder/apps`). This needs an internet connection and can take a few minutes: wait for the downloads shown in the status bar and the notifications to finish before opening the project.')
section('Linux only: USB access to the board', 'sec-env-usb')
P('Without these permissions, the board is not found by the tools and the serial port cannot be opened. Installing MPLAB X IDE also installs the udev rule, otherwise use the copy saved in the project:')
code(['sudo cp docs/dev-environment/linux/z010_mchp_tools.rules /etc/udev/rules.d/', 'sudo udevadm control --reload-rules && sudo udevadm trigger',
      'sudo usermod -aG dialout $USER     # access to /dev/ttyACM* (serial port)'])
P('Log out and back in (or reboot) for the group change to apply, then unplug and replug the board.')
section('Optional tools')
items(['**MPLAB X IDE v6.05:** used for command line programming (`ipecmd`, `mdb`) and to open the MPLAB X project files in `nbproject/`. Not needed to work in VS Code.',
       '**Python 3** with the packages of the microphone viewer: `pip install pyserial numpy matplotlib`.',
       '**MATLAB** (R2025a used, `serialport` is part of base MATLAB) for `tools/mic_viewer.m`.'])

chapter('Opening the Project', 'ch-env-open')
steps(['Plug in the Curiosity Nano board with a USB-C cable.',
       'In VS Code, use File > Open Folder and select the `CuriosityTrainer.X` folder.',
       'The MPLAB extension reads the project from `.vscode/CuriosityTrainer.mplab.json` and shows the project **CuriosityTrainer**, configuration **default**, in the MPLAB view.',
       'If the extension asks to download the device pack (dsPIC33CK-MC_DFP) or the tool pack (nEDBG_TP), accept: packs are stored in `~/.mchp_packs`.',
       "When asked for the tool, select the board's debugger, listed as **PKOB nano** with its serial number. The choice is saved in `.vscode/CuriosityTrainer.mplab-local.json`, which is specific to each computer and board."])
P('If the `.vscode` folder is missing (for example in a fresh copy of the repository), either copy the files of `docs/dev-environment/vscode` into `.vscode` (chapter {ref:ch-env-saved}), or import the MPLAB X project: open the Command Palette (Ctrl+Shift+P), type **Import**, choose the MPLAB X project import command and select the `CuriosityTrainer.X` folder.')
section('Project settings')
P('The project configuration (in the MPLAB project properties, or `.vscode/CuriosityTrainer.mplab.json`) must match these values:')
longtable(['Setting', 'Value'], [
    ['Device', 'dsPIC33CK64MC105'],
    ['Toolchain', 'XC16 v2.10 (property group `default-XC16`, provider `microchip.toolchains:xc16@2.10`)'],
    ['Device pack', 'dsPIC33CK-MC_DFP 1.11.412'],
    ['Tool', "PKOB nano (the board's nEDBG debugger)"],
    ['Optimization', '-O0, symbols enabled, all warnings'],
    ['Memory models', 'Small code, small data'],
    ['Source files', 'All `*.c`, `*.h` and `*.s` files of the project folder (new files are picked up automatically)'],
], col('0.25') + col('0.67'))

chapter('Build, Program and Debug', 'ch-env-build')
items(['**Build:** run the build task **CuriosityTrainer: default - Build** (Terminal > Run Build Task, or the build button of the MPLAB view). The output is `out/CuriosityTrainer/default.elf`.',
       '**Program the board:** in the Run and Debug view (Ctrl+Shift+D), start **Program CuriosityTrainer: default - Production**. It builds the production image and programs it without debugging.',
       '**Debug:** start **Launch CuriosityTrainer: default**. It builds, programs and stops at the start of `main()`; use the usual VS Code debug controls (breakpoints, step, variables).',
       "**Serial port:** the board's debugger also provides a virtual COM port (`/dev/ttyACM0` on Linux, a COM port on Windows). The examples use 9600 baud, the microphone example 115200 baud. Use the MPLAB Data Visualizer extension, any serial terminal or the viewers in `tools/`. Only one program can open the port at a time."])
note('Tip:', 'the first build after opening the project takes longer: the extension generates the CMake build files in `cmake/` and `_build/`.')

chapter('Saved Configuration', 'ch-env-saved')
P('The folder `docs/dev-environment` keeps this configuration with the project, so it is not lost when a computer is replaced:')
longtable(['File', 'Content'], [
    ['`environment.json`', 'Versions of the OS, VS Code, XC16, MPLAB X, packs, MPLAB tools, Python and MATLAB'],
    ['`vscode-extensions.txt`', 'The VS Code extensions with their exact versions'],
    ['`install-vscode-extensions.sh` / `.ps1`', 'Install the extensions of the list (Linux or macOS / Windows)'],
    ['`capture_environment.sh`', 'Regenerates `environment.json` and `vscode-extensions.txt` from the current computer'],
    ['`vscode/`', "The project's `.vscode` files: MPLAB project, launch configurations, settings"],
    ['`linux/z010_mchp_tools.rules`', 'Microchip udev rule for USB access (Linux)'],
], col('0.36') + col('0.56'))
section('Restoring the VS Code configuration')
P("Copy the files of `docs/dev-environment/vscode` into the project's `.vscode` folder. `CuriosityTrainer.mplab-local.example.json` only shows the format of the per-computer file: the extension creates the real `CuriosityTrainer.mplab-local.json` when you select the board.")
P('Differences with the original `.vscode` files, made so the copy works on any computer:')
items(['`settings.json`: `cmake.sourceDirectory` used an absolute path of the development computer, it now uses `${workspaceFolder}`.',
       '`launch.json`: only the configurations of the **default** project configuration are kept. The original also had entries for a **curiosity** configuration that does not exist in the project.'])
section('Refreshing the snapshot')
P('After updating VS Code, the extensions, the compiler or the packs, run the capture script on Linux and commit the updated files:')
code(['./docs/dev-environment/capture_environment.sh'])

chapter('Troubleshooting the Environment', 'ch-env-trouble')
P('Problems with the firmware itself are covered in chapter {ref:ch-common-problems}.')
for title, text in [
    ('The board is not found, or the serial port cannot be opened (Linux)', 'Check the udev rule and the dialout group (section {ref:sec-env-usb}), then unplug and replug the board. `lsusb` should list the debugger as `03eb:2175 Atmel Corp. nEDBG CMSIS-DAP`.'),
    ('"Unrecoverable error while communicating with the PKOB nano", USB errors -109 or -150', 'The debugger lost its USB connection: unplug and replug the board, then start again. Very high UART baud rates (1000000 and above) on the virtual COM port can crash the debugger, and it drops bytes from 500000 baud: keep the UART at 250000 baud or below.'),
    ('The compiler is not found or not offered as a toolchain', 'Check that XC16 v2.10 is installed in its default folder (section {ref:sec-env-xc16}), then restart VS Code so the toolchains extension finds it again.'),
    ('The device pack version does not match', 'The VS Code project uses dsPIC33CK-MC_DFP 1.11.412, while the MPLAB X project files (`nbproject/`) were made with 1.5.126. Both work: install the version the tool asks for, the packs can be installed side by side.'),
    ('The tools are still downloading, or the MPLAB view stays empty', 'On the first start, wait for the MPLAB extensions to finish downloading their tools (section {ref:sec-env-ext}), then reload the window (Ctrl+Shift+P, Developer: Reload Window).'),
]:
    section(title); P(text)

part1 = '\n'.join(L)

# ---------- Part II ----------
part2 = open(PART2, encoding='utf8').read()
part2 = part2.replace(r'\chapter{Common problems}', r'\chapter{Common problems}\label{ch-common-problems}', 1)
assert r'\label{ch-common-problems}' in part2
part2 = re.sub(r'\n\nSolution\n', lambda m: '\n\n' + r'\textbf{Solution}' + '\n', part2)
part2 = part2.replace('MPLAB have ab outdated RI4', 'MPLAB have an outdated RI4')  # typo in the .docx

# ---------- document ----------
PREAMBLE = r"""% Curiosity Trainer Manual
% KOMA-Script book: Part I = development environment (VS Code + MPLAB),
% Part II = programming reference of the firmware drivers.
% Build with:  latexmk -pdf curiosity-trainer-manual.tex   (or pdflatex/lualatex, run twice)
\documentclass[
  11pt,
  letterpaper,
  oneside,
  DIV=13,               % text area size: higher = smaller margins
  parskip=half,          % space between paragraphs instead of indentation
  headings=normal,
  chapterprefix=false,
  toc=listof,
  numbers=noenddot,
]{scrbook}

\usepackage{iftex}
\ifPDFTeX
  \usepackage[T1]{fontenc}
  \usepackage[utf8]{inputenc}
  \usepackage{lmodern}
  \usepackage{textcomp}
\else
  \usepackage{fontspec}
\fi
\usepackage[english]{babel}
\usepackage{microtype}
\usepackage{amsmath}
\usepackage{graphicx}
\usepackage{xcolor}
\usepackage{array}
\usepackage{booktabs}
\usepackage{tabularx}
\usepackage{longtable}
\usepackage{enumitem}
\usepackage{listings}
\usepackage{tcolorbox}
\usepackage[automark,headsepline]{scrlayer-scrpage}
\usepackage[hidelinks]{hyperref}
\usepackage{bookmark}

% ---------- colors and page style ----------
\definecolor{accent}{HTML}{1F4E79}
\definecolor{codebg}{HTML}{F4F4F4}
\definecolor{codeframe}{HTML}{D0D0D0}
\addtokomafont{disposition}{\color{accent}}
\addtokomafont{minisec}{\color{accent}}
\setkomafont{pageheadfoot}{\normalfont\small\color{black!60}}
\pagestyle{scrheadings}
\clearpairofpagestyles
\ihead{\leftmark}
\ohead{\pagemark}
\setcounter{secnumdepth}{2}
\setcounter{tocdepth}{2}

% ---------- lists, tables ----------
\setlist{itemsep=0.2em, topsep=0.3em}
\renewcommand{\arraystretch}{1.25}
% two-column function tables and header tables converted from the reference
\newenvironment{functiontable}[1]
  {\par\smallskip\noindent\small\tabularx{\linewidth}{#1}}
  {\endtabularx\par\medskip}
\newenvironment{datatable}[1]
  {\par\smallskip\noindent\small\tabularx{\linewidth}{#1}}
  {\endtabularx\par\medskip}

% ---------- code ----------
\lstdefinestyle{c}{
  language=C,
  basicstyle=\ttfamily\small,
  keywordstyle=\color{accent}\bfseries,
  commentstyle=\color{black!55}\itshape,
  stringstyle=\color{green!40!black},
  backgroundcolor=\color{codebg},
  frame=single, rulecolor=\color{codeframe},
  framesep=4pt, xleftmargin=6pt, xrightmargin=6pt,
  breaklines=true, columns=fullflexible, keepspaces=true,
  showstringspaces=false, upquote=true, tabsize=4,
}
\lstdefinestyle{shell}{style=c, language={}, keywordstyle={}}
\lstset{style=c}

% ---------- note box ----------
\newtcolorbox{notebox}{colback=accent!6, colframe=accent, boxrule=0pt, leftrule=3pt,
  arc=0pt, left=8pt, right=8pt, top=4pt, bottom=4pt, before skip=8pt, after skip=10pt}

\hypersetup{
  pdftitle={Curiosity Trainer Manual},
  pdfsubject={Development environment and programming reference},
}

\begin{document}

% ---------- title page ----------
\begin{titlepage}
  \centering
  \vspace*{2cm}
  \includegraphics[width=4cm]{figures/kettering-logo.png}\par
  \vspace{1.5cm}
  {\usekomafont{title}\color{accent}\Huge Curiosity Trainer Manual\par}
  \vspace{0.6cm}
  {\Large Development environment and programming reference\par}
  \vspace{0.4cm}
  {\large Curiosity Nano dsPIC33CK64MC105 training board\par}
  \vfill
  {\large Kettering University\par}
  \vspace{0.3cm}
  {\normalsize Osfield Gaga \& Jeremy Gooch\par}
  \vspace{0.3cm}
  {\normalsize\today\par}
\end{titlepage}

\frontmatter
\tableofcontents
\listoffigures

\mainmatter
"""

doc = (PREAMBLE
       + '\n' + r'\part{Development Environment}\label{part-environment}' + '\n'
       + part1
       + '\n\n' + r'\part{Programming Reference}\label{part-reference}' + '\n'
       + part2
       + '\n' + r'\end{document}' + '\n')

os.makedirs(os.path.join(OUTDIR, 'figures'), exist_ok=True)
shutil.copy(LOGO, os.path.join(OUTDIR, 'figures', 'kettering-logo.png'))
open(os.path.join(OUTDIR, 'curiosity-trainer-manual.tex'), 'w', encoding='utf8').write(doc)
print('wrote', os.path.join(OUTDIR, 'curiosity-trainer-manual.tex'), len(doc), 'chars')

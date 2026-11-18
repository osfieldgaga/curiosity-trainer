"""Convert the Curiosity Nano Programming Reference (.docx) body into LaTeX for the
KOMA-Script manual. Usage: python3 docx2tex.py <unpacked docx dir> <out .tex> <figures dir>"""
import os, re, shutil, sys
import xml.etree.ElementTree as ET

SRC, OUT, FIGDIR = sys.argv[1:4]
W = '{http://schemas.openxmlformats.org/wordprocessingml/2006/main}'
M = '{http://schemas.openxmlformats.org/officeDocument/2006/math}'
R = '{http://schemas.openxmlformats.org/officeDocument/2006/relationships}'
A = '{http://schemas.openxmlformats.org/drawingml/2006/main}'
WP = '{http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing}'

doc = ET.parse(os.path.join(SRC, 'word/document.xml')).getroot()
rels = {r.get('Id'): r.get('Target') for r in ET.parse(os.path.join(SRC, 'word/_rels/document.xml.rels')).getroot()}
numfmt = {'1': 'bullet', '2': 'decimal', '3': 'bullet'}  # numId -> format (checked in numbering.xml)

# image file -> (output name, caption); image4 is the logo, used on the title page
FIGS = {
    'image1.png': ('image2cpp-settings.png', 'image2cpp settings used to convert a 128 x 64 bitmap for the OLED screen'),
    'image5.png': ('mplabx-plugins.png', 'MPLAB X: the Tools > Plugins window'),
    'image2.png': ('mplabx-plugins-update.png', 'Check for updates, then update the plugins'),
    'image3.png': ('ics-pgd3.png', 'ICS set to PGD3 in config.c'),
}

# ---------- text escaping ----------
UNI = {'≈': r'$\approx$', '±': r'$\pm$', 'µ': r'\textmu{}', '−': r'$-$', '×': r'$\times$', '→': r'$\rightarrow$',
       '≥': r'$\geq$', '≤': r'$\leq$', '’': "'", '‘': '`', '“': '``', '”': "''", '–': '--', '—': '---',
       '\u00a0': '~', '°': r'\textdegree{}', '…': r'\dots{}', '•': r'\textbullet{}'}
def esc(s):
    out = []
    for k, ch in enumerate(s):
        if ch == '-' and k + 1 < len(s) and s[k + 1] == '-':  # literal "--" (e.g. --port), not a dash
            out.append('-{}'); continue
        if ch in '\\': out.append(r'\textbackslash{}')
        elif ch in '&%$#_{}': out.append('\\' + ch)
        elif ch == '~': out.append(r'\textasciitilde{}')
        elif ch == '^': out.append(r'\textasciicircum{}')
        elif ch in UNI: out.append(UNI[ch])
        else: out.append(ch)
    return ''.join(out)

def esc_tt(s):  # inside \texttt: also allow line breaks at underscores and dots
    t = esc(s).replace(r'\_', r'\_\allowbreak{}')
    return re.sub(r'([/.-])(?!\{\})', lambda m: m.group(1) + r'\allowbreak{}', t)

# ---------- math (only 5 equations, written by hand from their text) ----------
def omath_text(m):
    out = []
    for el in m.iter():
        if el.tag == M + 'sSub':
            base = ''.join(t.text or '' for t in el.find(M + 'e').iter(M + 't'))
            sub = ''.join(t.text or '' for t in el.find(M + 'sub').iter(M + 't'))
            out.append(f'{base}_{{{sub}}}')
            for t in el.iter(M + 't'): t.text = ''  # consumed
        elif el.tag == M + 't' and el.text:
            out.append(el.text)
    return ''.join(out).strip()
MATH = {
    'F_{PB}': r'F_{\mathrm{PB}}',
    'F_{T1}': r'F_{\mathrm{T1}}',
    'F_{PB} = Fosc / 2': r'F_{\mathrm{PB}} = \frac{F_{\mathrm{osc}}}{2}',
    'F_{PB} = 32 MHz / 2 = 16 MHz': r'F_{\mathrm{PB}} = \frac{32\,\mathrm{MHz}}{2} = 16\,\mathrm{MHz}',
    'F_{T1} = 16 MHz / 8 = 2 MHz, or 2,000,000 Hz': r'F_{\mathrm{T1}} = \frac{16\,\mathrm{MHz}}{8} = 2\,\mathrm{MHz} = 2\,000\,000\,\mathrm{Hz}',
}

# ---------- runs ----------
def run_props(r):
    rp = r.find(W + 'rPr')
    b = i = mono = False
    if rp is not None:
        e = rp.find(W + 'b'); b = e is not None and e.get(W + 'val') in (None, '1', 'true')
        e = rp.find(W + 'i'); i = e is not None and e.get(W + 'val') in (None, '1', 'true')
        f = rp.find(W + 'rFonts'); mono = f is not None and 'Consolas' in (f.get(W + 'ascii') or '')
    return b, i, mono

images_in_par = []
def inline(p):
    """LaTeX for the inline content of a paragraph (runs, math, links)."""
    parts = []
    def add_run(r):
        b, i, mono = run_props(r)
        txt = ''
        for c in r:
            if c.tag == W + 't': txt += c.text or ''
            elif c.tag == W + 'tab': txt += ' '
            elif c.tag == W + 'br': txt += '\n'
            elif c.tag == W + 'drawing':
                blip = c.find('.//' + A + 'blip')
                ext = c.find('.//' + WP + 'extent')
                images_in_par.append((rels[blip.get(R + 'embed')].split('/')[-1], int(ext.get('cx')) / 914400 if ext is not None else 6))
        if not txt: return
        lines = txt.split('\n')
        s = r'\\ '.join((esc_tt(l) if mono else esc(l)) for l in lines)
        if not s.strip(): parts.append(s); return
        if mono: s = r'\texttt{' + s + '}'
        if b: s = r'\textbf{' + s + '}'
        if i: s = r'\emph{' + s + '}'
        parts.append(s)
    for c in p:
        if c.tag == W + 'r': add_run(c)
        elif c.tag == W + 'hyperlink':
            rid = c.get(R + 'id')
            text = ''.join(t.text or '' for t in c.iter(W + 't'))
            if rid and rels.get(rid, '').startswith('http'): parts.append(r'\url{' + rels[rid] + '}')
            else:
                for r in c.findall(W + 'r'): add_run(r)
        elif c.tag == M + 'oMath':
            t = omath_text(c)
            if t: parts.append('$' + MATH.get(t, esc(t)) + '$')
    s = ''.join(parts)
    s = re.sub(r'\}\\textbf\{', '', s)  # merge adjacent bold runs
    s = re.sub(r'^(\s*\\\\\s*)+|(\s*\\\\\s*)+$', '', s.strip())  # no line break at the start or end
    return s.strip()

def ptext(p): return ''.join(t.text or '' for t in p.iter(W + 't'))
def pstyle(p):
    s = p.find(W + 'pPr/' + W + 'pStyle'); return s.get(W + 'val') if s is not None else ''
def numinfo(p):
    n = p.find(W + 'pPr/' + W + 'numPr')
    if n is None: return None
    return numfmt.get(n.find(W + 'numId').get(W + 'val'), 'bullet')
def is_display_math(p):
    ms = p.findall(M + 'oMath')
    return ms and not ptext(p).strip()

# ---------- tables ----------
def cell_paras(tc): return tc.findall(W + 'p')
def is_code_table(tbl):
    rows = tbl.findall(W + 'tr')
    if len(rows) != 1 or len(rows[0].findall(W + 'tc')) != 1: return False
    return any(run_props(r)[2] for r in tbl.iter(W + 'r'))
def code_text(tbl):
    lines = []
    for p in tbl.iter(W + 'p'):
        buf = ''
        for el in p.iter():
            if el.tag == W + 't': buf += el.text or ''
            elif el.tag == W + 'br': buf += '\n'
            elif el.tag == W + 'tab': buf += '    '
        lines.append(buf)
    txt = '\n'.join(lines).rstrip()
    for k, v in {'“': '"', '”': '"', '’': "'", '\u00a0': ' '}.items(): txt = txt.replace(k, v)
    return txt
def cell_tex(tc):
    paras = [inline(p) for p in cell_paras(tc)]
    return r'\newline '.join(x for x in paras if x)

def table_tex(tbl):
    rows = tbl.findall(W + 'tr')
    cells = [[cell_tex(tc) for tc in tr.findall(W + 'tc')] for tr in rows]
    ncol = max(len(r) for r in cells)
    first = [re.sub(r'\\[a-z]+\{|\}', '', c) for c in cells[0]]
    header = ncol >= 2 and first[0] in ('Pin', 'Stage', 'Byte offset', 'Register', 'Function')
    if ncol == 2 and not header:   # function tables: name | description
        spec = r'>{\raggedright\arraybackslash}p{0.36\linewidth}>{\raggedright\arraybackslash}X'
        body = [f'{r[0]} & {r[1]} \\\\' for r in cells]
        return '\n'.join([r'\begin{functiontable}{' + spec + '}', r'\toprule', *body, r'\bottomrule', r'\end{functiontable}'])
    if header:
        if first[0] == 'Pin':
            spec = r'>{\raggedright\arraybackslash}p{0.16\linewidth}>{\raggedright\arraybackslash}X>{\raggedright\arraybackslash}p{0.2\linewidth}'
        elif first[0] == 'Byte offset':
            spec = r'>{\raggedright\arraybackslash}p{0.22\linewidth}>{\raggedright\arraybackslash}p{0.14\linewidth}>{\raggedright\arraybackslash}X'
        else:
            spec = r'>{\raggedright\arraybackslash}p{0.22\linewidth}>{\raggedright\arraybackslash}X>{\raggedright\arraybackslash}X'
        head = ' & '.join(r'\textbf{' + re.sub(r'\\textbf\{(.*)\}', r'\1', c) + '}' for c in cells[0]) + r' \\'
        body = [' & '.join(r) + r' \\' for r in cells[1:]]
        return '\n'.join([r'\begin{datatable}{' + spec + '}', r'\toprule', head, r'\midrule', *body, r'\bottomrule', r'\end{datatable}'])
    spec = '|'.join(['X'] * ncol)
    return '\n'.join([r'\begin{functiontable}{' + spec + '}', r'\toprule', *[' & '.join(r) + r' \\' for r in cells], r'\bottomrule', r'\end{functiontable}'])

# ---------- walk the body ----------
out = []
emit = out.append
started = False
in_list = None          # 'itemize' / 'enumerate'
chapter_has_section = False
pending_fig = []

def close_list():
    global in_list
    if in_list: emit(r'\end{' + in_list + '}'); in_list = None

def figure(name, width_in):
    if name not in FIGS: return
    dst, cap = FIGS[name]
    shutil.copy(os.path.join(SRC, 'word/media', name), os.path.join(FIGDIR, dst))
    frac = min(1.0, width_in / 6.5)
    emit(r'\begin{center}')
    emit(rf'\includegraphics[width={frac:.2f}\linewidth]{{figures/{dst}}}')
    emit(r'\captionof{figure}{' + esc(cap) + '}')
    emit(r'\end{center}')

body = doc.find(W + 'body')
for el in body:
    if el.tag == W + 'p':
        st = pstyle(el); txt = ptext(el).strip()
        if not started:
            if st == 'Heading1' and txt == 'Introduction': started = True
            else: continue
        images_in_par.clear()
        if st.startswith('Heading'):
            close_list()
            if not txt: continue
            lvl = int(st[-1]); t = esc(txt)
            if lvl == 1:
                chapter_has_section = False
                emit('\n' + r'\chapter{' + t + '}')
            elif lvl == 2:
                chapter_has_section = True; emit('\n' + r'\section{' + t + '}')
            elif lvl == 3:
                emit('\n' + (r'\subsection{' if chapter_has_section else r'\section{') + t + '}')
            else:
                emit(r'\minisec{' + t + '}')
            continue
        if is_display_math(el):
            close_list()
            for m in el.findall(M + 'oMath'):
                t = omath_text(m)
                if t: emit(r'\[ ' + MATH.get(t, esc(t)) + r' \]')
            continue
        content = inline(el)
        kind = numinfo(el)
        raw = ptext(el)
        if kind:
            env = 'enumerate' if kind == 'decimal' else 'itemize'
            if in_list != env: close_list(); emit(r'\begin{' + env + '}'); in_list = env
            emit(r'\item ' + content)
            for name, w in images_in_par: figure(name, w)
            continue
        if raw.startswith('•'):  # manual bullets ("•  label text")
            if in_list != 'itemize': close_list(); emit(r'\begin{itemize}'); in_list = 'itemize'
            emit(r'\item ' + re.sub(r'^\\textbullet\{\}(~|\s)*', '', content))
            continue
        close_list()
        # a paragraph made only of bold text that keeps with the next one: a run-in sub-heading
        ppr = el.find(W + 'pPr')
        keep = ppr is not None and ppr.find(W + 'keepNext') is not None
        if keep and content.startswith(r'\textbf{') and content.endswith('}') and content.count(r'\textbf{') == 1:
            emit('\n' + r'\paragraph{' + content[8:-1] + '}')
            continue
        if content:
            emit('\n' + content)
        for name, w in images_in_par: figure(name, w)
    elif el.tag == W + 'tbl':
        if not started: continue
        close_list()
        if is_code_table(el):
            emit(r'\begin{lstlisting}')
            emit(code_text(el))
            emit(r'\end{lstlisting}')
        else:
            emit(table_tex(el))
close_list()

tex = '\n'.join(out)
tex = re.sub(r'\n{3,}', '\n\n', tex)
open(OUT, 'w', encoding='utf8').write(tex + '\n')
print('wrote', OUT, len(tex), 'chars')

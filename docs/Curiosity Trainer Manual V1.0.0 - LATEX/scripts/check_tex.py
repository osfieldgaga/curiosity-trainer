import re,sys,os
p=sys.argv[1]; t=open(p,encoding='utf8').read(); d=os.path.dirname(p)
body=re.sub(r'\\begin\{lstlisting\}.*?\\end\{lstlisting\}','',t,flags=re.S)
s=re.sub(r'\\[{}]','',body); s=re.sub(r'(?<!\\)%.*','',s)
depth=0; worst=0
for i,ch in enumerate(s):
    if ch=='{': depth+=1
    elif ch=='}':
        depth-=1
        if depth<0: print('negative brace depth near', repr(s[max(0,i-80):i+20])); break
print('final brace depth', depth)
stack=[]
for m in re.finditer(r'\\(begin|end)\{([^}]+)\}', re.sub(r'(\\begin\{lstlisting\}).*?(\\end\{lstlisting\})',r'\1\2',t,flags=re.S)):
    if m.group(1)=='begin': stack.append(m.group(2))
    else:
        top=stack.pop() if stack else None
        if top!=m.group(2): print('mismatch', top, m.group(2)); break
print('unclosed envs', stack)
labels=set(re.findall(r'\\label\{([^}]+)\}',t)); refs=set(re.findall(r'\\ref\{([^}]+)\}',t))
print('missing labels', refs-labels)
figs=re.findall(r'\\includegraphics\[[^\]]*\]\{([^}]+)\}',t)
print('figures', len(figs), 'missing', [f for f in figs if not os.path.exists(os.path.join(d,f))])
print('non-ascii outside listings', sorted(set(c for c in body if ord(c)>126)))
print('lone \\\\ lines', len(re.findall(r'^\s*\\\\\s*$',body,re.M)))
print('control characters (tabs etc.) outside listings', sorted(set(repr(c) for c in body if ord(c)<32 and c!='\n')))
print('chars', len(t), 'lines', t.count('\n'))

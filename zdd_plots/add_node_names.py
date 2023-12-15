'''
This script adds the node variable names for all the 
ZDD dot files in the same directory.

It uses regex to match and find all the variable names and their associated addresses. Then replaces all the addresses with the respective variable names. And finally rewrites the dot file.
'''

import os
import re
regex = re.compile(r'{ rank = same; " (.*?) ";\n(.*?)(?=})', re.S)

files = [f for f in os.listdir('.') if os.path.isfile(f) and f.endswith('.dot')]
for fname in files:
    with open(fname, 'r+') as f:
        txt = f.read()
        matches = regex.finditer(txt)
        for m in matches:
            g = m.groups()
            name = g[0]
            addr = g[1].strip().split('\n')
            if len(addr) == 1:
                txt = txt.replace(addr[0][:-1], f'"{name}"')
            else:
                for i, a in enumerate(addr):
                    txt = txt.replace(a[:-1], f'"{name}{chr(0x2080 + i)}"')
        f.seek(0)
        f.write(txt)
        f.truncate()

import re
from sys import argv

matcher = re.compile(r'^([0-9A-F]+) ;')

ttc = fontforge.open(argv[1])
for line in open('emoji-data.txt'):
    match = matcher.match(line)
    try:
        if match:
            val = int(match[1], 16)
            ttc.removeGlyph(val)
    except ValueError:
        pass
ttc.generate(argv[1])

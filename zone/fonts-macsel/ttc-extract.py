import fontforge
import sys
import os

for path in sys.argv[1:]:
    if not os.path.isfile(path):
        continue

    print(f"Processing: {path}")
    subfonts = fontforge.fontsInFile(path)

    if not subfonts:
        print("  No subfonts found.")
        continue

    print(f"  Found {len(subfonts)} subfonts: {', '.join(subfonts)}")
    base = os.path.splitext(path)[0]  # strips .ttc safely

    for name in subfonts:
        font = fontforge.open(f"{path}({name})")

        # Sanitize: replace spaces and other problematic chars with underscore
        safe_name = name.replace(" ", "_").replace("/", "_").replace("\\", "_")

        out = f"{safe_name}.ttf"
        font.generate(out)
        print(f"    Generated: {out}")
        font.close()

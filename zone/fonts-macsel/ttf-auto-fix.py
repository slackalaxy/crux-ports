#!/usr/bin/env python3
"""
fix_font_metrics.py -- Auto-fix vertical metrics for excessive line spacing

Focuses on OS/2 Typo metrics (most relevant for Linux/GTK/FreeType/Pango).
Removes invalid hhea attribute access.
"""

import fontforge
import sys
import os


def fix_metrics(font):
    em = font.em
    ascent = font.ascent
    descent = font.descent

    # Safety margin: 5–10% of ascent to avoid clipping on tall accents
    margin = max(40, int(ascent * 0.07))   # ~56 for your Ascent=800

    print(f"  Original:")
    print(f"    Em size:     {em}")
    print(f"    Ascent:      {ascent}")
    print(f"    Descent:     {descent}")
    print(f"    TypoAscent:  {font.os2_typoascent}")
    print(f"    TypoDescent: {font.os2_typodescent}")
    print(f"    TypoLineGap: {font.os2_typolinegap}")

    # Core fix: clamp Typo to general Ascent/Descent + margin
    font.os2_typoascent          = ascent + margin
    font.os2_typoascent_add      = False

    font.os2_typodescent         = -descent
    font.os2_typodescent_add     = False

    font.os2_typolinegap         = 0       # Already 0 in your case, but enforce

    # Tighten Win metrics (safety for Windows fallback)
    font.os2_winascent           = ascent + int(ascent * 0.15)  # ~920
    font.os2_windescent          = descent + int(descent * 0.5) # ~300

    # Crucial: Prefer Typo metrics (Linux apps will use our fixed Typo values)
    font.os2_use_typo_metrics    = True

    # Note: hhea metrics are NOT directly settable in Python API.
    #       FontForge usually copies Typo → hhea or derives during generate().
    #       With UseTypoMetrics=True, most renderers ignore hhea anyway.

    print(f"  After fix:")
    print(f"    TypoAscent:  {font.os2_typoascent}")
    print(f"    TypoDescent: {font.os2_typodescent}")
    print(f"    TypoLineGap: {font.os2_typolinegap}")
    print(f"    Effective line height ≈ {font.os2_typoascent - font.os2_typodescent}")


def main():
    if len(sys.argv) < 2:
        print("Usage: python3 fix_font_metrics.py font1.ttf [font2.ttf ...]")
        sys.exit(1)

    for path in sys.argv[1:]:
        if not os.path.isfile(path):
            print(f"File not found: {path}")
            continue

        print(f"\nProcessing: {path}")

        try:
            font = fontforge.open(path)
            print(f"  Font name: {font.fontname}")

            fix_metrics(font)

            base, ext = os.path.splitext(path)
            out_path = f"{base}-fixed{ext}"
            font.generate(out_path)
            print(f"  Saved: {out_path}")

            font.close()

        except Exception as e:
            print(f"  Error: {e}")


if __name__ == "__main__":
    main()
    
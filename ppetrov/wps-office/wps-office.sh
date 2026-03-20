#!/bin/bash

export FONTCONFIG_FILE="/etc/fonts/wps-office-fonts.conf"

# WPS Office seems to have trouble with absolute paths...
export LC_ALL=C
export QT_NO_THREADED_RENDERING=1
file="$1"

# Handle file:// URIs
if [[ $file == file://* ]]; then
    file="${file#file://}"
fi

# Convert absolute path to relative path if possible
if [[ $file == /* ]]; then
    cd "$(dirname "$file")" || exit 1
    file="$(basename "$file")"
fi

/usr/lib/wps-office/office6/wps "$file" > /dev/null 2>&1


# When we reach here, wps has exited... clean up zombies
# -q = quiet (no error if nothing found)
# -9 = forceful kill (SIGKILL); you can try without -9 first if you prefer graceful

echo "WPS closed. Waiting a moment for background processes to finish..." >&2
sleep 4

# Try graceful termination first
killall -q -15 wpscloudsvr wpsoffice wpspdf wps wpp et promecefpluginhost 2>/dev/null

sleep 1

# Force if necessary
killall -q -9 wpscloudsvr wpsoffice wpspdf wps wpp et promecefpluginhost 2>/dev/null

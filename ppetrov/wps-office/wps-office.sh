#!/bin/bash

#export LC_ALL=C
#export LC_ALL=en_US.UTF-8

# WPS Writer wrapper - fixes absolute path bug
export FONTCONFIG_FILE="/etc/fonts/wps-office-fonts.conf"
export QT_NO_THREADED_RENDERING=1
# Optional: quieter output
# export QT_LOGGING_RULES="*.warning=false"

file="$1"

# Strip file:// prefix if present
if [[ $file == file://* ]]; then
    file="${file#file://}"
fi

# If absolute path -> cd to directory and use only basename
if [[ -n "$file" && $file == /* ]]; then
    dir="$(dirname "$file")"
    base="$(basename "$file")"
    if cd "$dir" 2>/dev/null; then
        file="$base"
    fi
fi

# Launch WPS (replace shell with wps process)
exec /usr/lib/wps-office/office6/wps "$file" > /dev/null 2>&1


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

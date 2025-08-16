#!/bin/bash

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

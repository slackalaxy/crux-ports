#!/bin/bash
#
# trimssd: SSD trim using fstrim
#

CONFIG="/etc/trimssd.conf"
LOG="/var/log/trimssd.log"
DATE=$(date -R)

# Source configuration
if [ -f "$CONFIG" ]; then
    . "$CONFIG"
else
    echo "ERROR: Config file $CONFIG not found" >&2
    exit 1
fi

# Ensure LOG exists and is readable
if [ ! -f "$LOG" ]; then
    touch "$LOG" && chmod 644 "$LOG"
fi

echo "=== $DATE ===" >> "$LOG"

# Check if mountpoint array is set and not empty
if [ ${#mountpoint[@]} -eq 0 ]; then
    echo "ERROR: No mount points defined in $CONFIG" >> "$LOG"
    exit 1
fi

for mp in "${mountpoint[@]}"; do
    if mountpoint -q "$mp" 2>/dev/null; then
        if ! fstrim -v "$mp" >> "$LOG" 2>&1; then
            echo "WARNING: fstrim failed on $mp" >> "$LOG"
        fi
    else
        echo "WARNING: $mp is not mounted - skipping" >> "$LOG"
    fi
done

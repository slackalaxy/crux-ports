#!/bin/bash
#
# prtdiff: Update ports tree and log prt-get diff (for cron)
#

LOG="/var/log/prtdiff.log"
DATE=$(date -R)

PORTS="/usr/bin/ports"
PRTGET="/usr/bin/prt-get"

# Ensure log exists
if [ ! -f "$LOG" ]; then
    touch "$LOG" && chmod 644 "$LOG" || {
        echo "Failed to create $LOG" >&2
        exit 1
    }
fi

echo "=== $DATE ===" >> "$LOG"

# Silence normal output, log errors 
"$PORTS" -u > /dev/null 2>> "$LOG" || echo "WARNING: ports -u failed" >> "$LOG"

if ! "$PRTGET" quickdiff >> "$LOG" 2>&1; then
    echo "ERROR: prt-get quickdiff command failed" >> "$LOG"
fi

echo "" >> "$LOG"

# Keep only the last 2000 lines, so the log doesn't grow forever
tail -n 2000 "$LOG" > "$LOG.tmp" && mv "$LOG.tmp" "$LOG"

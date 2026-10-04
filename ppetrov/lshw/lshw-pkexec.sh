#!/bin/bash
# Run lshw-pkexec as root via pkexec while keeping access to the current
# X display.
#
# pkexec starts with a clean environment, so we must explicitly pass:
# DISPLAY -- which X server to use (e.g. :0.0)
# XAUTHORITY -- file containing the X authentication cookie

exec pkexec env \
  DISPLAY="$DISPLAY" \
  XAUTHORITY="${XAUTHORITY:-$HOME/.Xauthority}" \
  /usr/sbin/gtk-lshw

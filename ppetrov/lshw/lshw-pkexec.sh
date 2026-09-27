#!/bin/bash
exec pkexec env DISPLAY="$DISPLAY" XAUTHORITY="${XAUTHORITY:-$HOME/.Xauthority}" /usr/sbin/gtk-lshw

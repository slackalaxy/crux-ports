#!/bin/sh

# Force the system variables to register X11
export XDG_SESSION_TYPE=x11
export GDK_BACKEND=x11
export QT_QPA_PLATFORM=xcb

# Safely inject Chromium flags through the environment 
export QTWEBENGINE_CHROMIUM_FLAGS="--ozone-platform=x11 --disable-features=WaylandFractionalScale"

# Forward URL arguments natively to the binary launcher safely
exec /usr/lib/zoom-linux/ZoomLauncher "$@"

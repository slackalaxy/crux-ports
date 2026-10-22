#!/bin/bash

APPIMAGE="/usr/lib/viber/viber.AppImage"

# GIO_EXTRA_MODULES, GIO_MODULE_DIR: stop Viber's bundled GLib from
#   loading the system's GIO plugins (gvfs, dconf...), which are built
#   for a different GLib version and can cause warnings or crashes.

# QT_QPA_PLATFORM=xcb: run under X11, don't try Wayland.

# QML_DISABLE_DISTANCEFIELD=1: draw Qt Quick text with normal, hinted
#   font rendering instead of the soft, blurry default.

env \
  GIO_EXTRA_MODULES="" \
  GIO_MODULE_DIR="" \
  QT_QPA_PLATFORM=xcb \
  QML_DISABLE_DISTANCEFIELD=1 \
  "$APPIMAGE" "$@"

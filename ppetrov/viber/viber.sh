#!/bin/bash

APPIMAGE="/usr/lib/viber/viber.AppImage"

env \
  GIO_EXTRA_MODULES="" \
  GIO_MODULE_DIR="" \
  "$APPIMAGE"

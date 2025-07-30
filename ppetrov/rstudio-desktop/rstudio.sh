#!/bin/bash

# Disable VAAPI to avoid hardware acceleration issues. Enable software
# rendering as a fallback. Disable GPU acceleration.
export LIBVA_DRIVER_NAME=off
export LIBGL_ALWAYS_SOFTWARE=1
/usr/lib/rstudio/rstudio --disable-gpu "$@"

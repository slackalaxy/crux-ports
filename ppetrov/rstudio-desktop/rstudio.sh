#!/bin/bash

# Disable VAAPI to avoid hardware acceleration issues. Enable software
# rendering as a fallback. Disable GPU acceleration.
export LIBVA_DRIVER_NAME=off

# Disable GPU acceleration.
#export LIBGL_ALWAYS_SOFTWARE=1

# There seems to be an issue likely related to thread priority conflicts,
# possibly with how RStudio's mutexes (used in LinkBasedFileLock.cpp)
# interact with CRUX's threading system when initialized with certain
# attributes (e.g., PTHREAD_PRIO_PROTECT). The nice command lowers the
# process priority, which seems to avoid the mutex locking failure
# (pthread_mutex_lock: Invalid argument).
# nice -n 10 /usr/lib/rstudio/rstudio --disable-gpu "$@"
nice -n 10 /usr/lib/rstudio/rstudio "$@"
#!/usr/bin/env bash

# If not already running inside xterm → re-exec ourselves inside xterm
if [[ -z "$XTERM_VERSION" && -x /usr/bin/xterm ]]; then
    exec xterm -title "ProtoDeviser Shiny" -e "$0" "$@"
    exit
fi

R -q -e 'library(protodeviser); protodeviser_ui()'

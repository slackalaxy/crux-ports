#!/bin/bash
GUID_HOME=${GUID_HOME:-/usr/lib/guidance/www/Guidance}
export PATH=$GUID_HOME:$PATH
perl ${GUID_HOME}/guidance.pl "$@"

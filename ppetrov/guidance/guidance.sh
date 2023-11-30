#!/bin/bash
export GUID_HOME=${GUID_HOME:-/usr/lib/guidance/www/Guidance}
cd $GUID_HOME

perl ${GUID_HOME}/guidance.pl

#!/usr/bin/env bash

# Use this exact JDK for everything this script starts
export JAVA_HOME="/usr/lib/jdk"
export PATH="$JAVA_HOME/bin:$PATH"

exec "$JAVA_HOME/bin/java" \
    -Xms512m -Xmx1400m \
    -jar /usr/lib/sa/sa.jar "$@"

#!/bin/sh
exec java -Xms512m -Xmx1024m -jar /usr/lib/seq-ana/sa.jar "$@"

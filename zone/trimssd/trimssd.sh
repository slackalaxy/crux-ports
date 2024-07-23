#!/bin/bash
#
# trimssd: do file system trimming as a cron job
#
# read the mount points from /etc/trimcron.conf and run
# fstrim on them
#

. /etc/trimssd.conf

echo "*** $(date -R) ***" >> $LOG

for i in ${mountpoint[@]}; do
	fstrim -v $i >> $LOG
done

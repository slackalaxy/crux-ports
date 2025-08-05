#!/bin/bash

PORTS=($(prt-get listinst))

for P in ${PORTS[@]}; do
    if [[ "$(prt-get path ${P} 2>&1)" == *not\ found ]]
    then
        echo "${P} not found. (orphaned?)"
        continue
    fi
    COLL=$(basename $(dirname $(prt-get path ${P})))
    if [ ${COLL} != "core" ]; then
        if [ -z "$(prt-get dependent ${P})" ]; then
            echo ${COLL}/${P}
        fi
    fi
done


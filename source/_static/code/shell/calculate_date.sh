#!/bin/bash

# Author: Liu Hui
# 
# Date: Thu Jul  4 23:42:53 CST 2013


SECOND_TODAY=`date +%s`
SECOND_PER_DAY=86400

INTERVAL=7

SEQ=`seq $INTERVAL`

#set -x
for i in $SEQ
do
    DATE=`echo "$SECOND_TODAY - $SECOND_PER_DAY * ${i}" | bc`
    echo `date --date="@$DATE" +%Y%m%d`
done
#set +x

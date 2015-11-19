#!/bin/sh
SOURCE=/Users/bvgastel/dev/speedhttpd/lib
for file in {*.h,*.cpp}
do
    [ -f "$SOURCE/$file" ] && echo $file && cp $SOURCE/$file .
    for subdir in concurrency container crypto db io memory parser rx script streamingstore sync
    do
	[ -f "$SOURCE/$subdir/$file" ] && echo $subdir/$file && cp $SOURCE/$subdir/$file .
    done
done

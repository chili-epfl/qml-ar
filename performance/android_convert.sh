#!/bin/bash

if [ "X$1" == "X" ]
then
    echo "Usage: $0 input_raw.txt"
    exit 1
fi

cat $1 |grep "D libar-chest.so: (null):0 ((null)):"|cut -d " " -f 5- > ${1}_filtered.txt

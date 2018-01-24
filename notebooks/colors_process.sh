#!/bin/bash
cat colors_log.txt |grep HSV|cut -d $'\t' -f 5|cut -d ' ' -f 2->colors.txt

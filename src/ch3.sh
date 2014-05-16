#!/bin/bash

x=1
y=1

rm 'test'
touch 'test'

for x in {0..30}
do
	for y in {0..30}
	do
		echo ${x} ${y} `curl --data "x=${x}&y=${y}" http://gamblers.contest.tuenti.net/index.php 2>/dev/null | grep 'name="result"' | sed 's/.*name="result" value="\([0-9\.]*\)".*/\1/'` >> test
	done
done

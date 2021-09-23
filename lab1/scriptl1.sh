#!/bin/bash
rm -rf 9091

mkdir 9091
cd 9091
mkdir Semenov
cd Semenov
date > Slava
date --date="next Mon" > filedate.txt
cat Slava filedate.txt > result.txt
cat result.txt
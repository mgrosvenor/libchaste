#! /bin/bash

tmp=$1

rm -rf $tmp/include
find . -type d | grep -v "\.$" | grep -v ".\/include" | grep -v "\/bin" | grep -v "/\.git" | grep -v ".\/dag" | grep -v ".\/netmap" | grep -v "build" > $tmp/.tmp.dirs
find . | grep ".*.h$" | grep -v ".\/include" | grep -v "\/bin" | grep -v "/\.git" | grep -v ".\/dag" | grep -v ".\/netmap" | grep -v "build" > $tmp/.tmp.headers

python makeinclude.py $tmp 

rm $tmp/.tmp.headers
rm $tmp/.tmp.dirs


#!/bin/bash

path=$HOME
dirp=$HOME/lab1/11510352/file.info
mkdir -p $HOME/lab1/11510352/
touch $dirp
> $dirp

for dir in `find $path -type d -not -path '*/\.*'`
do
    echo "[ $dir ]" >> $dirp
    for file in `find $dir -mindepth 1 -maxdepth 1 -not -path '*/\.*'`
    do
    echo "$file" >> $dirp
    done
done
echo "[ Directories Count ] : "`find $path -mindepth 1 -type d -not -path '*/\.*'|wc -l` >> $dirp
echo "[ File Count ] : "`find $path -type f -not -path '*/\.*'|wc -l` >> $dirp

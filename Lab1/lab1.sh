#!/bin/bash

# set IFS
SAVEIFS=$IFS
IFS=$(echo -en "\n\b")

path=$HOME
dirp=$HOME/lab1/11510352/file.info
mkdir -p $HOME/lab1/11510352/
touch $dirp
> $dirp

directory_count=0
file_count=0

for dir in `find $path -type d -not -path '*/\.*'`
do
    echo "[ $dir ]" >> $dirp
    let directory_count++
    for file in `find $dir -mindepth 1 -maxdepth 1 -not -path '*/\.*'`
    do
    let file_count++
    echo "$file" >> $dirp
    done
    echo "" >> $dirp
done
echo "[ Directories Count ] : "$directory_count >> $dirp
echo "[ File Count ] : "$file_count >> $dirp

# restore $IFS
IFS=$SAVEIFS

#!/bin/bash

if [ $# -ne 4 ]; then
    echo Wrong arguments number
    exit -1
elif [ $2 -lt 0 ]; then
    echo Number of files can not be negative
    exit -1
elif [ $3 -lt 0 ]; then
    echo Number of directories can not be negative
    exit -1
elif [ $4 -gt 5 ]; then
    echo Maximun number of levels is 5
    exit -1
elif [ $4 -gt $3 ]; then
    echo Number of directories must be greater than or equal to levels
    exit -1
fi

if [ ! -d $1 ]; then
    mkdir -p $1
fi

declare -a dirs
declare -a files

let current_dir_count=$3
cd $1
root=`pwd`
dirs[0]=${root}

while [ true ]
do

    for ((lvl = 0; lvl <= $4; lvl++))
    do
        rand=$((1 + RANDOM % 8))
        dir=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $rand | head -n 1)

        if [ ! -d $dir ]; then
            mkdir -p $dir
        fi

        cd $dir
        current_dir=`pwd`
        dirs[${#dirs[@]}]=${current_dir}
        ((current_dir_count--))

        if [ $current_dir_count -eq 0 ];then
            break
        fi

    done

    cd $root

    if [ $current_dir_count -eq 0 ];then
        break
    fi

done

let current_file_count=$2

while [ true ]
do

    for ((dr = 0; dr <= "${#dirs[@]}-1" ; dr++))
    do
        rand=$((1 + RANDOM % 8))
        file=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $rand | head -n 1)

        rand=$((1 + RANDOM % 128))

        current_dir=${dirs[dr]}
        cd ${current_dir}
        files[${#files[@]}]="${current_dir}/$file"
        $(cat /dev/urandom | tr -dc 'a-zA-Z0-9!@#$%&*\n\r\t\ ' | fold -w $(($rand*1000)) | head -n 1 > ${files[${#files[@]} - 1]}) 
        
        ((current_file_count--))

        if [ $current_file_count -eq 0 ];then
            break
        fi

    done

    if [ $current_file_count -eq 0 ];then
        break
    fi

done
#!/bin/bash

declare -a clients_connected
declare -a clients_disconnected
is_leave=0
idx=0
bytes_sent=0
bytes_read=0
files_sent=0
files_received=0

while read -a cols;
do
    is_leave=0
    idx=0
    if [ ${cols[0]} = 'connected' ]; then
        if [[ ! " ${clients_connected[@]} " =~ " ${cols[1]} " ]];then
            clients_connected[${#clients_connected[@]}]=${cols[1]}
        fi
        elif [ ${cols[0]} = 'disconnected' ];then
        if [[ ! " ${clients_disconnected[@]} " =~ " ${cols[1]} " ]];then
            clients_disconnected[${#clients_disconnected[@]}]=${cols[1]}
        fi
    else
        if [ ${cols[0]} = 1 ];then
            ((bytes_sent+=${cols[3]}))
        else
            ((bytes_read+=${cols[3]}))
        fi

        if [ ${cols[0]} = 1 ] && [ ${cols[5]} = 0 ];then
            ((files_sent++))
        elif [ ${cols[0]} = 0 ] && [ ${cols[5]} = 0 ];then
            ((files_received++))
        fi
    fi
done

echo
echo Clients that connected are :
( IFS=$' '; echo "${clients_connected[*]}" )
echo

echo The number of clients that disconnected is : ${#clients_disconnected[@]}

sorted=( $( printf "%s\n" "${clients_connected[@]}" | sort -n ) )

echo The minimum id of a client was: ${sorted[0]} and the maximum ${sorted[-1]}

echo The sum of bytes read is: $bytes_read

echo The sum of bytes wrote is: $bytes_sent

echo The number of files sent is: $files_sent

echo The number of files reseived is: $files_received

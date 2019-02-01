declare -A STATS
total=0
for FILENAME in ./log/*;
do
    while read p;
    do
        SEARCH_TERM=$(echo $p|cut -d':' -f5 )
        NUM_RESULTS=$(echo $p|cut -d':' -f6- |tr ':' '\n'|wc -l) 
        if [ ${STATS[$SEARCH_TERM]} ]
        then
            B=${STATS[$SEARCH_TERM]}
            STATS[$SEARCH_TERM]=$(( B+NUM_RESULTS ))
        else
            STATS[$SEARCH_TERM]=$NUM_RESULTS
        fi
    done < $FILENAME
done
max=0
min=0
for i in "${!STATS[@]}"
do
    n=${STATS[$i]}
    ((n > max)) && max=$n && max_key=$i 
    ((n < min)) && min=$n && min_key=$i 
    ((min == 0)) && min=$n && min_key=$i 
    total=$((total+1));
done
echo "Total number of keywords searched : $total"
echo "$min_key [totalNumFilesFound $min]"
echo "$max_key [totalNumFilesFound $max]"


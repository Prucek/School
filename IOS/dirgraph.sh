#!/usr/bin/env bash

POSIXLY_CORRECT=yes

FSHIST() #1list_of_sizes
{   
    declare -a list
    list[0]=100 
    list[1]=1024 
    list[2]=10240 
    list[3]=102400 
    list[4]=1048576 
    list[5]=10485760 
    list[6]=104857600 
    list[7]=1073741824

    declare -a to_print
    to_print[0]="<100 B  "
    to_print[1]="<1 KiB  "
    to_print[2]="<10 KiB "
    to_print[3]="<100 KiB" 
    to_print[4]="<1 MiB  "
    to_print[5]="<10 MiB " 
    to_print[6]="<100 MiB" 
    to_print[7]="<1 GiB  "
    to_print[8]=">=1 GiB "

    arg=$@
    sum=0
    for i in {0..8}
    do
        count=0
        for j in $arg
        do
            if test $i -eq 8; then
                if test $j -gt ${list[7]}; then
                    ((count++))
                fi 
            elif test $j -lt ${list[i]}; then
                ((count++))
            fi
            shift
        done
    
        count=$((count-sum))
        sum=$((sum+count))

        echo -n "  ${to_print[i]}: "
        for (( k=1; k<=$count; k++ ))
        do
            echo -n \#
        done
        echo
    done

}

#printing
DIR=$(pwd)
ND=$(ls -lR | grep "^d" | wc -l)
NF=$(find $(pwd) -type f | wc -l)
echo Root directory: $DIR
echo Directories: $ND 
echo All files: $NF
echo File size histogram:

list_of_sizes=$(ls -lR | grep "^-" | sed -E 's/([^0-9]*([0-9]*)){2}.*/\2/')
FSHIST $list_of_sizes

return 0
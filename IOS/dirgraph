#!/usr/bin/env bash

POSIXLY_CORRECT=yes
OPTIND=1
DIR=$(pwd)
n=0
i=0


#checking arguments
while getopts ni: opt; do
    case $opt in
        n) n=1
        ;;
        i) i=1
        ;;
        \?) printf "Wrong\n" # ADD ERROR    TODO to stderr
            exit 1
        ;;
    esac
done

if test $n -eq 1;then
    shift
fi

if test $i -eq 1;then
    shift
    shift
fi


if test $# -gt 1 ; then
    #more arguments
    printf "Wrong\n" # ADD ERROR TODO to stderr
    exit 1
elif test $# -eq 1; then
    if test -d $1;then 
        DIR=$1
    else
        #not a directory
        printf "Wrong\n" #ADD ERROR TODO to stderr
        exit 1
    fi
fi

#ADD ERROR IF LOCKED FILES TODO 



#File size histogram
FSHIST() #$1 DIR
{   

    list_of_sizes=$(ls -lR $DIR | grep "^-" | sed -E 's/([^0-9]*([0-9]*)){2}.*/\2/')

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

    arg=$list_of_sizes
    sum=0
    len=${#list[@]}

    for((i=0; i<=$len; i++))
    do
        #calculating count of # to print
        count=0
        for j in $arg
        do
            if test $i -eq $len; then
                if test $j -gt ${list[$len-1]}; then
                    ((count++))
                fi 
            elif test $j -lt ${list[i]}; then
                ((count++))
            fi
            shift
        done

        if test $i -ne $len; then
            count=$((count-sum))
            sum=$((sum+count))
        fi

        #checking if terminal
        if [ -t 1 ]; then
            width=$(($(tput cols)-1))
        else
            width=79
        fi

        #if normalization (-n) is on
        if test $NF -gt $width && test $n -eq 1; then
            count=$((count/(NF/width)))
        fi

        #printing length-#
        printf "  ${to_print[i]}: "
        for (( k=1; k<=$count; k++ ))
        do
            printf "#"
        done
        echo
    done
}


#printing
ND=$(ls -lR $DIR| grep "^d" | wc -l)
NF=$(ls -lR $DIR| grep "^-" | wc -l)
printf "Root directory: $DIR\n"
printf "Directories: $ND\n"
printf "All files: $NF\n"
printf "File size histogram:\n"

FSHIST $DIR

exit 0

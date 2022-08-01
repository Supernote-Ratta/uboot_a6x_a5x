#!/bin/bash


## build uboot ##
option=$1
function makeuboot()
{
    if [ "$option" = "installclean" ]
        then
        make clean
    elif [ "$option" = "clean" ]
        then
        make clean
        make mrproper
    fi

    ./make.sh evb-px30_sn
}

makeuboot $*
if [ $? -ne 0 ]; then
    echo -e "\033[40;31m build uboot error \033[0m"
    exit 1
fi


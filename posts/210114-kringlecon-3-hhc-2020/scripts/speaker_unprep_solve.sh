#!/bin/bash

charset=({a..z} {A..Z} {0..9})

for i in "${charset[@]}"; do
    rm -f vending-machines.json
    ./vending-machines <<EOF
Can$i
Can$i
Can$i
EOF
    password=$(cat vending-machines.json | grep password | cut -d'"' -f4)
    echo $password
    # target password: LVEdQPpBwr
    if [ $password == "LVEd" ] ;
    then
        echo "####" $i "for password" $password
        exit 0
    else
        echo "trying " $i
    fi
 done

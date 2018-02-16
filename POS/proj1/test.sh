#!/bin/bash

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

output=$(./$1 $2 $3 | awk '{print $1}')
tickets=($output)
number=-1
pass=true
for ticket in "${tickets[@]}"
do
	if [ "$number" -ge "$ticket" ]; then
		echo "break"
		pass=false
		break
	fi
    number=$ticket
done

if $pass; then
	echo "[${green}OK${reset}]"
else
	echo "[${red}FAIL${reset}]"
fi

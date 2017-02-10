#!/bin/bash
graphEnabled=false
identificators=false
while getopts gpr:d: opt
do case "$opt" in
  
    g) graphEnabled=true;;
    r) requires=$OPTARG;;
    d) dependency=$OPTARG;;
    p) identificators=true;;
    *) echo "Zle zadane parametre." >&2

    exit 1;;
    esac
done
# Ulozenie vystupu do premenny a grepnutie len riadkov s callq alebo nazvom funckie
dump=$(eval objdump -d -j .text \${$OPTIND} | grep -E 'callq[[:space:]]*[[:xdigit:]]*[[:space:]]*<[[:alnum:]@_]*>|^[[:xdigit:]]* <.*>:')
if [ "$identificators" = false ]; then
	#Odstranenie riadkov s $plt
	dump=$(echo "$dump" | grep -v '@plt>$')
fi
if [ "$graphEnabled" = true ]; then
	echo "digraph CG {"
	dump=$(echo "$dump" | sed 's/@plt/_PLT/')
fi
# Citanie riadok po riadku
vystup=$(while read -r line; do
    if [[ $line =~ callq ]]; then
        callee=$(echo $line | awk '{print $NF}'| tr -d '<>' )
        if [ -n "$dependency" ]; then
            if [ "$caller" != "$dependency" ]; then
                continue
            fi
        elif [ -n "$requires" ]; then
            if [ "$callee" != "$requires" ]; then
                continue
            fi
        fi
        if [ "$graphEnabled" = false ]; then
            echo "$caller -> $callee"
        else
            echo "$caller -> $callee;"
        fi
    else
        caller=$(echo $line | awk '{print $2}' | tr -d '<>:')
    fi
done <<< "$dump")
vystup=$(echo "$vystup" |sort -u )
if [ -n "$vystup" ]; then
    echo "$vystup"
fi
if [ "$graphEnabled" = true ]; then
    echo "}"
fi

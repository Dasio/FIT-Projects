#!/bin/bash
vypiszavislost()
{
    zavislost=""
    obsahuje=$(nm $2|grep '\s*[TBCDG]' | awk '{print $3}')
    if [ -n "$vyzaduje" ];then
        zavislost=$(echo "$obsahuje" |grep "$vyzaduje")
    fi
    caller="$1"
    callee="$2"
    if [ "$graphEnabled" = true ] ; then
        if [ -n "$zavislost" ];then
            if [[ ! "$uzly" =~ "$caller" ]]; then
                uzly+="$caller\n"
            fi
            if [[ ! "$uzly" =~ "$callee" ]]; then
                uzly+="$callee\n"
            fi
        fi
        caller=$(echo "$caller" |sed 's/.*\///'|sed 'y/-\.\+/_DP/')
        callee=$(echo "$callee" |sed 's/.*\///'|sed 'y/-\.\+/_DP/')
    fi
    for word in $zavislost
    do
         if [ "$graphEnabled" = true ] ; then
               echo "$caller -> $callee [label=\"$word\"];"
         else
            echo "$caller -> $callee ($word)"
         fi
    done
}
graphEnabled=false;
while getopts gr:d: opt

do case "$opt" in

    g) graphEnabled=true;;
    r) requires=$OPTARG;;
    d) dependency=$OPTARG;;
    *) echo "Zle zadane parametre." >&2

    exit 1;;

    esac

done
# Nemozu byt zadane obe moznosti
if [ -n "$requires" -a -n "$dependency" ]; then
    echo "Moze byt bud -r alebo -d"
   exit 1;
fi
if [ "$graphEnabled" = true ] ; then
    echo "digraph GSYM {"
fi
# Prechadza kazdy subor s kazdym
for i in `seq $OPTIND $#`; do
    # Zisti ci bol zadany -d
    if [ -n "$dependency" ]; then
        kontrola=$(eval echo \${$i} |sed 's/.*\///')
        if eval [ "$dependency" != "$kontrola" ]; then
            continue
        fi
    fi
    # Spravi grep ktore vypise len zavislosti a awk zoberie z toho treti stlpec
    # Sed da nazaciatok kazdeho slova ^ a na konci $ pre neskorsi grep
    vyzaduje=$(eval nm \${$i}|grep '\s*U'| awk '{print $2}' |sed 's/^/^/' | sed 's/$/$/') 
    for j in `seq $OPTIND $#`; do
                # Nemoze vypisovat zavislost sam so sebou
                if [ "$i" -eq "$j" ] ; then
                        continue
                fi
                if  [ -n "$requires"  ]; then
                        kontrola=$(eval echo \${$j} |sed 's/.*\///')
                        if [ "$requires" != "$kontrola" ]; then
                            continue
                        fi
                fi
                eval vypiszavislost \${$i} \${$j}
    done
done
if [ "$graphEnabled" = true ] ; then
    uzly=$(echo -ne $uzly)
    for uzol in $uzly ; do
        true
        echo -n $uzol| sed 'y/-\.\+/_DP/'
        echo " [label=\"$uzol\"];"
    done
    echo "}"
fi

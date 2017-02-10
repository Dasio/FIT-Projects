#!/bin/bash
#
PASS="[ \033[0;32mPASS\033[0;0m ]"
FAIL="[ \033[0;31mFAIL\033[0;0m ]"
reset=`tput sgr0`
cyan=`tput setaf 6`
index=1
runserver ()
{
    run="./ipkhttpserver -p 1337"
    # Chunk size
    if [ "$1" ]; then
        run+=" -c $1"
    fi
    # Min-time for chunk
    if [ "$2" ]; then
        run+=" -t $2"
    fi
    $run &
    serverid=$!
    # Wait for init server
    sleep 0.2
}
result ()
{
    if [ $? == 0 ]; then
        printf "$PASS";
    else
        printf "$FAIL";
    fi
    echo
}
killserver ()
{
    kill -9 $serverid
    wait $serverid 2>/dev/null
}

setup ()
{
    mkdir -p ./www
    echo "<html> <body> Nejaky index abcdefgijklmnopqrstuvwxzy </body>  Next abcdefgijxklmnopqrstuvwxzy </html>" > ./www/index.html
    dd if=/dev/urandom of=./www/1mb.txt bs=1024 count=1000 >& /dev/null
    dd if=/dev/urandom of=./www/10mb.txt bs=10240 count=1000 >& /dev/null
    # Just for browser
    mkdir ./www/foo/
    echo "random" > ./www/foo/random.txt
    echo "random2" > ./www/foo/random2.txt
}

teardown ()
{
    rm -rf ./www/foo
    rm -f ./www/1mb.txt
    rm -f ./www/10mb.txt
    rm -f ./www/index.html
}
msg ()
{
    echo -en "[Test $index] $1"
    ((index++))
}
keepalivetest ()
{
    out=$(ab -n $1 -c $2 -k localhost:1337/ 2> /dev/null | grep requests:)
    IFS=$'\n' lines=($out)
    IFS=$' \t\n'
    complete=$(echo ${lines[0]} | awk '{print $3}')
    success=$(echo ${lines[2]} | awk '{print $3}')
    [ $complete = $success ]
    msg "Keep-Alive $1 requests, $2 requests perform at a time. Test by ApacheBench\t"
    result
    echo "$out"
}
runtests ()
{
    echo "${cyan}Argument tests${reset}"
    out=$(./ipkhttpserver -h)
    msg "Server --help\t\t\t\t\t\t\t\t\t"
    [[ -n $out ]]
    result
    echo "$out"

    msg "Server -c 12b - return code 1\t\t\t\t\t\t\t"
    ./ipkhttpserver -c 12b 2> /dev/null
    [ $? = 1 ]
    result

    echo "${cyan}Server without any argument${reset}"
    runserver
    msg "Get root directory, should return index.html\t\t\t\t\t"
    ./ipkhttpclient localhost:1337
    payload=$(ls -t | grep payload | head -1)
    out=$(ls -t | grep out | head -1)
    in=$(ls -t | grep in | head -1)
    diff ./www/index.html $payload > /dev/null
    result
    cat "$out"
    cat "$in"
    echo "${cyan}Re-run server, to have valid .out.log, because next files contains random data ${reset}"
    killserver
    sleep 0.5
    runserver
    msg "Get 1mb.txt - Compare payload and original file with diff\t\t\t"
    getfile 1mb.txt
    msg "Get 10mb.txt - Compare payload and original file with diff\t\t\t"
    getfile 10mb.txt

    msg "Try to get file which doesnt exists, in header should be status code 404\t"
    ./ipkhttpclient localhost:1337/nofile
    header=$(ls -t | grep header | head -1)
    cat -v $header | head -1 | grep 404 > /dev/null
    result
    cat -v "$header"
    echo

    msg "Directory listing, check if something is in payload (better view in browser)\t"
    ./ipkhttpclient localhost:1337/foo
    payload=$(ls -t | grep payload | head -1)
    [[ -s $payload ]]
    result
    cat $payload
    echo

    keepalivetest 500 5
    killserver

    echo "${cyan}Server with -c 50 ${reset}"
    runserver 50
    msg "Get root directory, should return index.html\t\t\t\t\t"
    ./ipkhttpclient localhost:1337
    payload=$(ls -t | grep payload | head -1)
    diff ./www/index.html $payload > /dev/null
    result
    out=$(ls -t | grep out | head -1)
    in=$(ls -t | grep in | head -1)
    cat "$out" "$in"
    msg "Get 1mb.txt - Compare payload and original file with diff\t\t\t"
    getfile 1mb.txt
    msg "Get 10mb.txt - Compare payload and original file with diff\t\t\t"
    getfile 10mb.txt

    msg "Try to get file which doesnt exists, in header should be status code 404\t"
    ./ipkhttpclient localhost:1337/nofile
    header=$(ls -t | grep header | head -1)
    cat -v $header | head -1 | grep 404 > /dev/null
    result
    cat -v $header
    echo

    msg "Directory listing, check if something is in payload (better view in browser)\t"
    ./ipkhttpclient localhost:1337/foo
    payload=$(ls -t | grep payload | head -1)
    [[ -s $payload ]]
    result
    killserver

    echo "${cyan}Server with -c 5000 -t 10 ${reset}"
    runserver 5000 10
    msg "Get root directory, should return index.html\t\t\t\t\t"
    ./ipkhttpclient localhost:1337
    payload=$(ls -t | grep payload | head -1)
    diff ./www/index.html $payload > /dev/null
    result
    out=$(ls -t | grep out | head -1)
    in=$(ls -t | grep in | head -1)
    cat "$out" "$in"

    msg "Get 1mb.txt - Compare payload and original file with diff\t\t\t"
    getfile 1mb.txt
    msg "Try to get file which doesnt exists, in header should be status code 404\t"
    ./ipkhttpclient localhost:1337/nofile
    header=$(ls -t | grep header | head -1)
    cat -v $header | head -1 | grep 404 > /dev/null
    result
    cat -v $header
    echo

    msg "Directory listing, check if something is in payload (better view in browser)\t"
    ./ipkhttpclient localhost:1337/foo
    payload=$(ls -t | grep payload | head -1)
    [[ -s $payload ]]
    result
    killserver

}

getfile ()
{
    ./ipkhttpclient localhost:1337/$1
    payload=$(ls -t | grep payload | head -1)
    diff ./www/$1 $payload > /dev/null
    result
}

if [ ! -f ipkhttpserver -o ! -f ipkhttpclient ]; then
    make bins
fi
make cleanlogs
setup
runtests
teardown

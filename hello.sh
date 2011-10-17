#!/bin/bash

echo 1 + 1, $((1+1)), $[1+1], $[3/4]
# bash only use int, but bc can evaluate arithmetic expressions
echo 3/4 | bc -l

## stderr and stdout
rmdir aaa 2> test.out     # stderr to file
grep Hello * 1>& 2        # stdout to stderr
grep Hello * >& /dev/null # stdout and stderr to file, null means absdute silence
echo ----
ls -l | sed -e "s/[aeio]/u/g"  # sed: stream editor for filtering and tranforming text

## VARIABLES: no need to declare, just assing a value to its reference to create 
STR="Hello World!"        # cannot use spece character between = or inside a no "" string
echo $STR                 # get the value of STR
echo STR                  # what will happen?
## echo the output of a shell instruction
echo $(ls)                # will show the result of 'ls'
OF=/var/my-backup-$(date +%Y%m%d).tgz
echo $OF
echo ----

## function and local variables
HELLO=Hello
function hello {          # define a function
  local HELLO=local_hello # local variables
  echo $HELLO
}
echo $HELLO
hello                     # call a function
echo $HELLO

function e {
  echo $1
}
e AAAA
e BBBB
echo ----

## for loop, 2 types
for i in $( ls ); do
  echo item: $i
done
for i in `seq 1 10`; do
  echo seq:$i
done
echo ----

## while and until
COUNTER=0
while [ $COUNTER -lt 5 ]; do
  echo The counter is $COUNTER
  let COUNTER=COUNTER+1
done
echo current counter $COUNTER
until [ $COUNTER -lt 2 ]; do
  echo counter untile $COUNTER
  let COUNTER-=1
done
echo ----

## command line
if [ -z "$1" ]; then
  echo usage: $0 directory
#  exit
fi
SRCD=$1
TGTD="/var/backups/"
OF=home-$(date +%Y%m%d).tgz
echo tar -cZf $TGTD$OF $SRCD
echo ----

# get return value, assume the dir is not exist
cd /abcdefg >& /dev/null
echo rv: $?
cd $(pwd) &> /dev/null
echo rv: $?

## selection
#### if conditionals: if [exp_1]; then; stat_1; elif; [exp_2]; then; stat_2; else; stat_3;fi
###### read to get user input
OPTT="Hello Quit"
select opt in $OPTT; do
  if [ "$opt" = "Quit" ]; then
    echo done
    exit
  elif [ "$opt" = "Hello" ]; then
    echo Please, wrote your firstname and lastname 
    read FN LN
    echo "Hi! $LN, $FN!"
    echo 
  else
    echo bad option
  fi
done
echo ----



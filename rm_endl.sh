#!/bin/bash

git status | grep "modified" > ll

for i in $(cat ll);do
  if [ $i = "modified:" ];then
    echo "pass $i"
  else
    echo "do $i"
    cat $i | sed -e "s/\r//" > out
    mv out $i
  fi
done

rm ll


#!/bin/sh

rm log.txt
cc main.c -o  main && ./main  > log.txt

cat log.txt

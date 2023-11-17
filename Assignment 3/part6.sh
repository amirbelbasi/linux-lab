#!/bin/bash

read -p "Enter x: " x
read -p "Enter y: " y
read -p "Enter file name: " file
i=1
while IFS= read -r line
do
    [[ $((i++)) -ge $x ]] && echo "$line"
    [[ $i -gt $y ]] && exit 0
done < "$file"
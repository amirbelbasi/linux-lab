#!/bin/bash

legal_values='^[+-]?[0-9]+$'
for i in "${@}"; do
	if [[ !($i =~ $legal_values) ]]; then
		echo "Non numeric value found: $i"
		exit -1
	fi
done

if [[ $# -ne 2 ]]; then
	echo "Illegal number of arguments"
	exit -1
fi

echo "Sum: $(($1 + $2))"
if [[ $1 -gt $2 ]]; then echo "Greater value: $1"
elif [[ $2 -gt $1 ]]; then echo "Greater value: $2"
else echo "$1 and $2 are equal"
fi
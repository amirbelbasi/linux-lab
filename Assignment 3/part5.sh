#!/bin/bash

while [[ "$inp" -ne -1 ]]; do
	read -p "Enter your number: " inp
	inp_holder=$inp
	sum_holder=0
	while [[ "$inp_holder" -ne 0 ]]; do
		let sum_holder+=inp_holder%10
		let inp_holder=inp_holder/10
	done
	if [[ $inp -ne -1 ]]; then
		echo "Reversed: $(echo $inp | rev)"
		echo "Sum: $sum_holder"
	fi
done
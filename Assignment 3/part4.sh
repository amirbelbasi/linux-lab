#!/bin/bash

read -p "First operand: " operand1
read -p "Second operand: " operand2
read -p "Operator: " operator
case $operator in
	+) echo "Output: $(($operand1 + $operand2))";;
	-) echo "Output: $(($operand1 - $operand2))";;
	\*) echo "Output: $(($operand1 * $operand2))";;
	/) echo "Output: $(($operand1 / $operand2))";;
esac
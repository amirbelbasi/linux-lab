#!/bin/bash

read -p "First operand: " operand1
read -p "Second operand: " operand2
read -p "Operator: " operator
case $operator in
  +) result=`echo $operand1 + $operand2 | bc`;;
  -) result=`echo $operand1 - $operand2 | bc`;;
  \*) result=`echo $operand1 \* $operand2 | bc`;;
  /) result=`echo "scale=3; $operand1 / $operand2" | bc`;;
esac
echo "$result"
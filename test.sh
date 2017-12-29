#!/bin/bash
# 
# File:   test.sh
# Author: marko
#
# Created on Nov 22, 2017, 8:32:21 PM
#

# Simple bash test
# loops throu a rang of numbers, feeds the current value in the loop to generator
# generator in return generates 2 random string with the passed length and
# stores them in input.txt
# main.exe is then executed and caluclates the difference of the two strings in 
# file input.txt


# default values
step=1
max_length=10000
min_length=1000

# check input parameters
if [ $# -eq 0 ] 
then

# if no params echo usage msg, continue working with default params
    echo  "Change test params by invoking test like -> ./test.sh <min_length> <max_length> <step>."
else

# set input parameters
    min_length=$1
    max_length=$2  
    step=$3
fi

echo "Staring test: string length range $min_length - $max_length step $step"
echo ""

make clean
make

# loop given range 
for (( x=$min_length; x<=$max_length; x+=$step ))
do

	echo "**************************************************************"
	echo "Testing input with $x characters"
	
# execute generator with the current value in the loop, generates two strings in input.txt
	./generator $x
	
# read input.txt and calculate edit distance
	./main
	echo ""
done
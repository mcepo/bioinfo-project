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

test_length=50

# check input parameters
if [ $# -eq 0 ]
then

# if no params echo usage msg, continue working with default params
    echo  "Change test params by invoking test like -> ./test.sh <test_length>"
else

# set input parameters
    min_length=$1
    max_length=$2
    step=$3
fi

echo "Staring test: number of tests  $test_length"
echo ""

make clean
make

# loop given range
for (( x=1; x<=$test_length; x+=1 ))
do

	echo "**************************************************************"
	echo "******* TEST $x *******"
# execute generator with the current value in the loop, generates two strings in input.txt
	./generator

# read input.txt and calculate edit distance
	./main
	echo ""
done

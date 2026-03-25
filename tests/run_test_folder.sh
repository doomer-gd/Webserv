#!/bin/bash

#first argument should be the name of the testfile and folder

test_name=$1
dir_name=$(dirname $(realpath $0))/input/$test_name
cd $dir_name
#pwd > test_data
#ls | grep -v test_data >> test_data
bin_name=$dir_name/../../bin/$test_name
for test_file in $dir_name/*.txt; do
	printf "$test_file"
	$bin_name $test_file
	echo ;
done

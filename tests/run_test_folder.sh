#!/bin/bash

#first argument should be the name of the testfile and folder

test_name=$1
dir_name=$(dirname $(realpath $0))/input/$test_name
cd $dir_name
pwd > test_data
ls | grep -v test_data >> test_data
../../bins/$test_name $dir_name/test_data

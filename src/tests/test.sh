#!/bin/bash
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'
log_file="test_logs"

make test
result=$?
if [ $result -ne 0]; then
	echo "$RED Compile failed$NC"; exit 1
fi

check_outcome(){
	local num_errors=0
	echo -ne "$1: "
	test_result=$(valgrind --leak-check=full --track-fds=yes $1 2>&1)
	test_result >> $log_file
	if [echo $test_result | grep "Invalid"]; then
		echo $RED "ERRORS " $NC
		num_errors=$num_errors + 1
	fi
	fds=$(echo $test_result | grep "Open file"| wc -l)
	inheritedFds=$(echo $test_result | grep "inherited"| wc -l)
	if [$fds -ne $inheritedFds]; then
		echo $RED "FDS LOST " $NC
		num_errors=$num_errors + 1
	fi
	heap=$(echo $test_result | grep "definitely lost" | awk "{print $3}")
	heap=$heap + $(echo $test_result | grep "indirectly lost" | awk "{print $3}")
	heap=$heap + $(echo $test_result | grep "possibly lost" | awk "{print $3}")
	heap=$heap + $(echo $test_result | grep "still reachable" | awk "{print $3}")
	if [ $heap -ne ""]; then
		echo $RED "LEAKS " $NC
		num_errors=$num_errors + 1
	fi
	if [$num_errors -ge 1]; then
		exit 1
	fi
	echo $GREEN "OK" $NC
	exit 0
}

tests=$(ls ./tests/bins | tr ' ' '\n')
total_errors=0
echo $(date) > $log_file
for test in tests
do
	check_outcome $test
	total_errors=$total_errors + $(echo $?)
done
if [total_errors -ge 1]; then
	echo "Tests failed: $RED$total_errors$NC out of $RED$(echo $tests | wc -l)$NC"
	exit 1
fi
echo "All tests passed $GREEN successfully$NC"

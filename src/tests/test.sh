#!/bin/bash

make test
result=$?
if [ $result -ne 0]; then
	echo "Compile failed"; exit 1
fi

check_outcome(){
	test_result=$(valgrind --leak-check=full --track-fds=yes $1)
	if [echo $test_result | grep "Invalid"]; then
		echo \033[0;31m "ERRORS "
	fi
	fds=$(echo $test_result | grep "Open file"| wc -l)
	inheritedFds=$(echo $test_result | grep "inherited"| wc -l)
	if [$fds -ne $inheritedFds]; then
		echo \033[0;31m "FDS LOST "
	fi
	heap=$(echo $test_result | grep "definitely lost" | awk "{print $3}")
	heap=$heap + $(echo $test_result | grep "indirectly lost" | awk "{print $3}")
	heap=$heap + $(echo $test_result | grep "possibly lost" | awk "{print $3}")
	heap=$heap + $(echo $test_result | grep "still reachable" | awk "{print $3}")
	if [ $heap -ne ""]; then
		echo \033[0;31m "LEAKS "
	fi
}

tests=$(ls ./bins | tr ' ' '\n')
for test in tests
do
	 test
done

#include "iostream"
#include "vector"
#include "stdio.h"

int main()
{
	FILE*	fakeFile;
	char*	fakeArray = new char[10];

	fakeFile = fopen("bad_file", "w");
	std::cout << fakeArray[10] << std::endl;
	return 0;
}

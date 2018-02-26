#include <stdio.h>
#include <stdlib.h>

const char* FILE_NAME = "addresses.txt";
const int BUFFER_SIZE = 256;

int main (int argc, char* argv[]){
	
	if (argc < 2){
		printf("Not enough arguments\nProgram Exiting\n");
		exit(0);
	}

	FILE *fd;
	fd = fopen(argv[1], "r");
	
	if (fd == NULL){
		printf("File failed to open\n");
		exit(0);
	}

	char readBuf[BUFFER_SIZE];
	memset(readBuf, 0, sizeof(readBuf));

	while (fgets(readBuf, BUFFER_SIZE, fd) != NULL){
		printf("%s\n", readBuf);
	}

	return 0;

}

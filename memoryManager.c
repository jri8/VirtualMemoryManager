#include <stdio.h>
#include <stdlib.h>


/*------TODO---------
Structure:
  Read file containing logical addresses
  Use TLB and Page Table
    Translate logical address to physical address

Read in
  File contains 32-bit ints
  Mask the right most 16 bits
    Divided into 8 bit page number (8-15)
                 8 bit offset (0-7)
Page Table - 2^8 entries
 
TLB - 16 Entries

Page Size - 2^8 bytes

Frame Size - 2^8 bytes

Physical memory - 256 frames x 256 frames

1. Write program that extracts page number and offset from following ints:
1, 256, 32768, 32769, 128, 65534, 33153

2. Bypass TLB and implement using page table

3.  Implement TLB, must have replacement strategy (FIFO or LRU)

--------TODO----------*/


const int PAGE_TABLE_SIZE = 256;
const int BUFFER_SIZE = 256;
const int PHYS_MEM_SIZE = 256;
const int TLB_SIZE = 16;


	return 1;
}

int findPage(int logicalAddr, unsigned char* PT){

	unsigned char mask = 0xFF;
	unsigned char offset;
	unsigned char pageNum;

	pageNum = (logicalAddr >> 8) & mask;
	printf("%X\t", pageNum);	

	offset = logicalAddr & mask;
	printf("%X\n", offset);


	return 0;
}




int main (int argc, char* argv[]){
	
	int val;
	FILE *fd;
	
	unsigned char PageTable[PAGE_TABLE_SIZE];
	unsigned char TLB[TLB_SIZE];	
	unsigned char PhyMem [PHYS_MEM_SIZE][PHYS_MEM_SIZE]; 

	if (argc < 2){
		printf("Not enough arguments\nProgram Exiting\n");
		exit(0);
	}

	fd = fopen(argv[1], "r");
	if (fd == NULL){
		printf("File failed to open\n");
		exit(0);
	}


	printf("Value\tPageNum\tOffset\n ");	
	while (fscanf(fd, "%d", &val)==1){
		printf("%X\t", val);
		findPage(val, PageTable);	
		
	
	}
//	readFromDisk();

	close(fd);
	return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int readFromDisk (int pageNum, char *PM, int* OF){

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, sizeof(buffer));

	FILE *BS;
	BS = fopen("BACKING_STORE.bin", "rb");
	if (BS == NULL){
		printf("File failed to open\n");
		exit(0);
	}
	
//	printf("offset in fseek: %d\n", pageNum*PHYS_MEM_SIZE);

	if (fseek(BS, pageNum * PHYS_MEM_SIZE, SEEK_SET)!=0)
		printf("error in fseek\n");

	if (fread(buffer, sizeof(char), PHYS_MEM_SIZE, BS)==0)
		printf("error in fread\n");
	

	int i = 0;
	for(i; i < PHYS_MEM_SIZE; i++){
		*((PM+(*OF)*PHYS_MEM_SIZE)+i) = buffer[i];
	/*	printf("buffer[%d]=%d\n",i+pageNum*256, buffer[i]);*/
	}
	
	(*OF)++;

	return (*OF)-1;
//	printf("&d\n ", *((PM+i*n)+j));
	// i = current row row number, n = elements per row, j = colum num
}





int findPage(int logicalAddr, char* PT, char* PM, int* OF){

	unsigned char mask = 0xFF;
	unsigned char offset;
	unsigned char pageNum;

	pageNum = (logicalAddr >> 8) & mask;
	printf("%X\t", pageNum);	

	offset = logicalAddr & mask;
	printf("%X\t", offset);
	
	//Check if in TLB

	//Check if in PageTable

	if (PT[pageNum] != NULL)
		printf("Pagehit\t\t");
//	PT[pageNum] = offset;
	
	
	//if not in either read from disk
	else{
		printf("pageFault\t");
		PT[pageNum] = readFromDisk(pageNum, PM, OF);
	}
	int value = PT[pageNum];
	int index = (value*PHYS_MEM_SIZE)+offset;
	value = *(PM+index);
	printf("value = %d at PM[%d]\n",value,index);	
	
	return 0;


}




int main (int argc, char* argv[]){
	
	int val;
	FILE *fd;
	int openFrame = 0;
	
	unsigned char PageTable[PAGE_TABLE_SIZE];
//	memset(PageTable, NULL, sizeof(PageTable));	
	int i =0;
	for (i;i < PAGE_TABLE_SIZE;i++)
		PageTable[i] = NULL;

	unsigned char TLB[TLB_SIZE];	
//	memset(TLB, NULL, sizeof(TLB));

	char PhyMem[PHYS_MEM_SIZE][PHYS_MEM_SIZE]; 

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
		printf("%d\t", val);
		findPage(val, PageTable, (char*)PhyMem, &openFrame);	
	}

//	readFromDisk(0, (char*)PhyMem, &openFrame);
	
	printf("openFrame = %d\n", openFrame);
/*	int i = 0;
	for (i; i < PHYS_MEM_SIZE; i++)
		printf("PhyMem[%d]=%d\n",i, PhyMem[0][i]);
*/
	close(fd);
	return 0;

}

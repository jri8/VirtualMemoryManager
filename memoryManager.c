#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

struct TLB {
	unsigned char TLBpage[16];
	unsigned char TLBframe[16];
	int ind;
};
	



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





int findPage(int logicalAddr, char* PT, struct TLB *tlb,  char* PM, int* OF, int* pageFaults, int* TLBhits){

	unsigned char mask = 0xFF;
	unsigned char offset;
	unsigned char pageNum;
	bool TLBhit = false;
	int frame = 0;
	int value;
	int newFrame = 0;

	printf("Virtual adress: %d\t", logicalAddr);

	pageNum = (logicalAddr >> 8) & mask;
//	printf("%X\t", pageNum);	

	offset = logicalAddr & mask;
//	printf("%X\t", offset);
	
	//Check if in TLB
	int i = 0;
	for (i; i < TLB_SIZE; i++){
		if(tlb->TLBpage[i] == pageNum){
			frame = tlb->TLBframe[i];
			TLBhit = true;
			(*TLBhits)++;
		//	printf("TLBhit\t\t");
		}
			
	}

	//Check if in PageTable
	if (TLBhit == false){
		if (PT[pageNum] != -1){
		//	printf("Pagehit\t\t");
		}

		//if not in either read from disk
		else{
	//		printf("pageFault\t");
			newFrame = readFromDisk(pageNum, PM, OF);
			PT[pageNum] = newFrame;
			(*pageFaults)++;
//			tlb->TLBpage[tlb->ind] = pageNum;
//			tlb->TLBframe[tlb->ind] = newFrame;
//			tlb->ind = (tlb->ind + 1)%TLB_SIZE;
		}
		frame = PT[pageNum];
		tlb->TLBpage[tlb->ind] = pageNum;
		tlb->TLBframe[tlb->ind] = PT[pageNum];
		tlb->ind = (tlb->ind + 1)%TLB_SIZE;
		
	}
	int index = ((unsigned char)frame*PHYS_MEM_SIZE)+offset;
	value = *(PM+index);
	printf("Physical address: %d\t Value: %d\n",index, value);	

	
	return 0;


}




int main (int argc, char* argv[]){
	
	int val;
	FILE *fd;
	int openFrame = 0;

	int pageFaults = 0;
	int TLBhits = 0;
	int inputCount = 0;
	
	float pageFaultRate;
	float TLBHitRate;

	unsigned char PageTable[PAGE_TABLE_SIZE];
	memset(PageTable, -1, sizeof(PageTable));	

	struct TLB tlb;	
	memset(tlb.TLBpage, -1, sizeof(tlb.TLBpage));
	memset(tlb.TLBframe, -1, sizeof(tlb.TLBframe));
	tlb.ind = 0;

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


	//printf("Value\tPageNum\tOffset\n ");	
	while (fscanf(fd, "%d", &val)==1){
	//	printf("%d\t", val);
		findPage(val, PageTable, &tlb, (char*)PhyMem, &openFrame, &pageFaults, &TLBhits);
		inputCount++;
	}

//	readFromDisk(0, (char*)PhyMem, &openFrame);
	
/*	int i = 0;
	for (i; i < PHYS_MEM_SIZE; i++)
		printf("PhyMem[%d]=%d\n",i, PhyMem[0][i]);
*/
	pageFaultRate = (float)pageFaults / (float)inputCount;
	TLBHitRate = (float)TLBhits / (float)inputCount;
	printf("Page Fault Rate = %.4f\nTLB hit rate= %.4f\n",pageFaultRate, TLBHitRate);
	close(fd);
	return 0;

}

/*
 * Blake Turner
 * 4 May 2016
 * Project 2, Task 1
 * */
#include "disk.h"
#include <time.h>
#include <math.h>
#include <stdio.h>

/*
    C = LBA ÷ (HPC × SPT)
    H = (LBA ÷ SPT) mod HPC
    S = (LBA mod SPT) + 1
*/
disk_t disk;
int log_to_phys(int logaddr, physaddr_t* phaddr){
	int code = 0;
	int cylinder = 0;
	int head = 0;
	int sector = 0;
	cylinder = logaddr / (NUM_OF_HEADS * NUM_OF_SECTS);
	head = (logaddr / NUM_OF_SECTS) % NUM_OF_HEADS;
	sector = (logaddr % NUM_OF_SECTS);
	phaddr->cyl = cylinder;
	phaddr->head = head;
	phaddr->sect = sector;
	return code;
}
int phys_to_log(physaddr_t* phaddr) {
	int logicalAddr = 0;
	logicalAddr = ((phaddr->cyl)*NUM_OF_HEADS + (phaddr->head)) * NUM_OF_SECTS + (phaddr->sect);
	return logicalAddr;
}
int write(int logical_block_num, int num_of_sectors, void *buffer){
	printf("Testing write()...\n");
	physaddr_t tmpPhAddr;
	log_to_phys(logical_block_num, &tmpPhAddr);

	if((logical_block_num + num_of_sectors) > MAX_LOGICAL_SECTOR || logical_block_num < 0){
		//invalid addresss
		printf("ERROR:phys_to_log(&tmpPhAddr2) > MAX_LOGICAL_SECTOR\n");
		return -1;
	}

	int i = 0;
	for(i = 0; i < num_of_sectors; i++){
		log_to_phys(logical_block_num+i, &tmpPhAddr);
		///printf("%d %d %d %x %x %d\n", tmpPhAddr.cyl, tmpPhAddr.head, tmpPhAddr.sect, disk[tmpPhAddr.cyl][tmpPhAddr.head][tmpPhAddr.sect], buffer, strlen(disk[tmpPhAddr.cyl][tmpPhAddr.head][tmpPhAddr.sect]));
		//printf("1:%s\n", disk[tmpPhAddr.cyl][tmpPhAddr.head][tmpPhAddr.sect]);
		//printf("2:%s\n", buffer);
		memcpy(disk[tmpPhAddr.cyl][tmpPhAddr.head][tmpPhAddr.sect], buffer, SECT_SIZE);
	}
	return 0;
}
/**
This function copies num_of_sectors sectors from the disk to the buffer starting with sector pointed to by logical_block_num.
You should validate the parameters before serving the request.
**/
int read(int logical_block_num, int num_of_sectors, void **buffer){
	printf("Testing read()...\n");
	physaddr_t tmpPhAddr;
	log_to_phys(logical_block_num, &tmpPhAddr);
	if(tmpPhAddr.cyl > NUM_OF_CYLS)
		return -1;
	if(tmpPhAddr.head > NUM_OF_HEADS)
		return -1;
	if(tmpPhAddr.sect > NUM_OF_SECTS)
		return -1;
	*buffer = (char*) malloc(num_of_sectors * SECT_SIZE);
	int i;
	for(i = 0; i < num_of_sectors; i++){
		log_to_phys(logical_block_num+i, &tmpPhAddr);
		memcpy(*buffer, disk[tmpPhAddr.cyl][tmpPhAddr.head][tmpPhAddr.sect], SECT_SIZE); //how to iterate through buffer?
	}
	return 0;
}
/*
int main(int argc, char* argv[]){

	physaddr_t physAddr;
	time_t t;
	srand(time(&t));
	int i = 0;
	int logicalAddress;
	do{ //working!
		physAddr.cyl = rand() % NUM_OF_CYLS;
		physAddr.head = rand() % NUM_OF_HEADS;
		physAddr.sect = rand() % NUM_OF_SECTS;
		//dbg rand() vals:
		//printf("cyl:%d head:%d sect:%d\n", physAddr.cyl,physAddr.head,physAddr.sect);
		logicalAddress = phys_to_log(&physAddr);
		//printf("%d\n", logicalAddress);
		log_to_phys(logicalAddress, &physAddr);
		//printf("cyl:%d head:%d sect:%d\n", physAddr.cyl,physAddr.head,physAddr.sect);
		i++;
	} while(i < 100);

	char *str = malloc(SECT_SIZE);
	strcpy(str, "All work and no play makes Jack a dull boy.\n");

	write(logicalAddress, 1, (void*)str);
	//printf("Done\n");
	void** buffer;
	read(logicalAddress, 1, &buffer);
	printf("%s", buffer);
	return 0;
}*/

/*
 * Blake Turner
 * Project 2, Task 2
 * May 9, 2016
 * */
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include "disk.h"

#define COUNTDOWN_VALUE 5

timer_t gTimerid;
int count = COUNTDOWN_VALUE;
disk_t disk;
void initializeDisk()
{
	//is there a faster way?
	int i, j, k, l;
	for(i = 0; i < NUM_OF_CYLS; i++)
		for(j = 0; j < NUM_OF_HEADS; j++)
			for(k = 0; k < NUM_OF_SECTS; k++)
				for(l = 0; l < SECT_SIZE; l++)
					disk[i][j][k][l] = '#';

}
void testDisk()
{
	physaddr_t physAddr;
	int logicalAddress;
	int isRead = rand() % 1;
	physAddr.cyl = rand() % NUM_OF_CYLS;
	physAddr.head = rand() % NUM_OF_HEADS;
	physAddr.sect = rand() % NUM_OF_SECTS;
	//dbg rand() vals:
	//printf("cyl:%d head:%d sect:%d\n", physAddr.cyl,physAddr.head,physAddr.sect);
	logicalAddress = phys_to_log(&physAddr);
	//printf("%d\n", logicalAddress);
	/*
	 * insert read/write testing here.
	 * */
	int randSectSize = rand() % 10;
	if(isRead == 1){
		void **buffer;
		read(logicalAddress, randSectSize, &buffer);
		printf("BUFFER CONTAINS:\n%s", buffer);
	} else if(isRead == 0) {
		//then we are writing
		char *str = malloc(SECT_SIZE*(randSectSize)+1);
		char data[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		int j = 0;
		int dataSize = rand() % (SECT_SIZE*randSectSize);
		for(j = 0; j < dataSize; j++){
			int index = rand() % (sizeof(data)-1); //hazardous line.. segfaulting cause of this limit.
			str[j] = data[index];
		}
		str[dataSize] = '\0';
		write(logicalAddress, randSectSize, (void*)str);
		//then verify stuff was actually written
		void** buffer;
		read(logicalAddress, randSectSize, &buffer);
		printf("SHOULD SEE RANDOM DATA:\n%s", buffer);
	}
	log_to_phys(logicalAddress, &physAddr);
	//printf("cyl:%d head:%d sect:%d\n", physAddr.cyl,physAddr.head,physAddr.sect);
	return;

}

void start_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 1;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 1;
    value.it_interval.tv_nsec = 0;

    timer_create (CLOCK_REALTIME, NULL, &gTimerid);

    timer_settime (gTimerid, 0, &value, NULL);

}

void stop_timer(void)
{
    struct itimerspec value;

    value.it_value.tv_sec = 0;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = 0;
    value.it_interval.tv_nsec = 0;

    timer_settime (gTimerid, 0, &value, NULL);
}

void timer_callback(int sig)
{
    printf("Caught timer signal: %d ... !!\n", sig);
    testDisk();
    count--;
}

int main(int ac, char **av)
{
	initializeDisk();
	time_t t;
	srand(time(&t));
    (void) signal(SIGALRM, timer_callback);
    start_timer();
    while(count >= 0);
    return 0;
}

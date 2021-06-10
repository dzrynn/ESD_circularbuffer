#include "mbed.h"
#include "cmsis_os.h"    
/* #include "Board_LED.h" */

#define CBUFFER_SIZE 8
#define DATA_SIZE 9

int CBUFFER[CBUFFER_SIZE];
int data[DATA_SIZE] = {1,2,3,4,5,6,7,8,9};
int output [DATA_SIZE];
int cbufferHead = 0;
int cbufferTail = 0;
int i = 0;
int j = 0;

void producer_thread (
	/*---
		wait or get event
		check if empty???
		----*/
	osSemaphoreWait(doneConsume, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	CBUFFER[cbufferTail] = data[i];
	i++;
    if cbufferTail = 7 
    (
        cbufferTail = 0;
    )
    else cbufferTail++;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneProduce);
)

void consumer_thread (
	osSemaphoreWait(doneProduce, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	output[j] = CBUFFER[cbufferHead];
	j++;
    /* how to remove value from buffer?? cannot delete element from array*/
    if cbufferHead = 7
    (
        cbufferHead = 0;
    )
	else cbufferHead++;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneConsume);
)

void main (
		doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
		doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), 0);	
		buffMutex = osMutexCreate(osMutex(buffMutex));	
)

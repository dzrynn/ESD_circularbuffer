#include "mbed.h"
#include "cmsis_os.h"    
/* #include "Board_LED.h" */

#define CBUFFER_SIZE 8

int CBUFFER[CBUFFER_SIZE];
int data[9] = {1,2,3,4,5,6,7,8,9};
int output [9];
int cbufferIndex = 0;
int i = 0;
int j = 0;
/* int readIndex = 0;
int writeIndex = 0; */

void producer_thread (
	/*---
		wait or get event
		check if empty???
		head & tail
		----*/
	osSemaphoreWait(doneConsume, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	CBUFFER[cbufferIndex] = data[i];
	cbufferIndex++;
	i++;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneProduce);
)

void consumer_thread (
	osSemaphoreWait(doneProduce, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	output[j] = CBUFFER[cbufferIndex];
	j++;
	cbufferIndex--;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneConsume);
)

void main (
		doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
		doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), 0);	
		buffMutex = osMutexCreate(osMutex(buffMutex));	
)

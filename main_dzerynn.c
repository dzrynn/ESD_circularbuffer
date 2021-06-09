#include "mbed.h"
#include "cmsis_os.h"    
#include "Board_LED.h"

#define CBUFFER_SIZE 8

int CBUFFER[CBUFFER_SIZE];
int cbufferIndex = 0;
/* int readIndex = 0;
int writeIndex = 0; */

void producer_thread (
	/*---
		wait or get event
		----*/
	osSemaphoreWait(doneConsume, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	/*---
		adding to buffer at CBUFFER[cbufferIndex]
		---*/
	cbufferIndex++;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneProduce);
)

void consumer_thread (
	osSemaphoreWait(doneProduce, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	/*---
		reading from buffer at CBUFFER[cbufferIndex]
		---*/
	cbufferIndex--;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneConsume);
)

void main (
		doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
		doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), 0);	
		buffMutex = osMutexCreate(osMutex(buffMutex));	
)

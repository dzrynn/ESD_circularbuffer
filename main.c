#include "cmsis_os.h"
#include "stm32f10x.h"

void producer_thread (void const *argument);
void consumer_thread(void const *argument);

// Define threads
osThreadDef(producer_thread, osPriorityNormal, 1, 0);
osThreadDef(consumer_thread, osProrityNormal,1,0);

// Define the semaphores

osSemaphoreId doneProduce;
osSemaphoreDef(doneProduce);
osSemaphoreId doneConsume;
osSemaphoreDef(doneConsume);

// Define the mutex

osMutexId buffMutex;
osMutexDef(buffMutex);

#define CBUFFER_SIZE 8
#define DATA_SIZE 9

static int CBUFFER[CBUFFER_SIZE];
static int data[DATA_SIZE] = {1,2,3,4,5,6,7,8,9};
static int output [DATA_SIZE];
static int cbufferHead = 0;
static int cbufferTail = 0;
static int i = 0;
static int j = 0;

void main (

	osKernelInitialize ();

		doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
		doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), 0);	
		buffMutex = osMutexCreate(osMutex(buffMutex));
			
	osKernelStart ();
)

void producer_thread (
	/*---
		wait or get event (control speed f data??)
		check if empty & full???
		----*/
  /*  if cbufferHead = (cbufferTail % (CBUFFER_SIZE-1)) + 1     //buffer full
    (

    )	*/
	osSemaphoreWait(doneConsume, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	CBUFFER[cbufferTail] = data[i];
	i++;
  cbufferTail = (cbufferTail+1) % (CBUFFER_SIZE-1);
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneProduce);
)

void consumer_thread (

/*    if cbufferHead = cbufferTail
    (

    )	*/
	osSemaphoreWait(doneProduce, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	output[j] = CBUFFER[cbufferHead];
	j++;
    /* how to remove value from buffer?? cannot delete element from array*/
    cbufferHead = (cbufferHead+1) % (CBUFFER_SIZE-1);
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneConsume);
)



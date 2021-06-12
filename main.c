#include "cmsis_os.h"
#include "stm32f10x.h"
#include "uart.h"

void producer_thread (void const *argument);
void consumer_thread(void const *argument);

// Define threads
osThreadDef(producer_thread, osPriorityNormal, 1, 0);
osThreadDef(consumer_thread, osPriorityNormal,1,0);

// Define the semaphores

osSemaphoreId doneProduce;
osSemaphoreDef(doneProduce);
osSemaphoreId doneConsume;
osSemaphoreDef(doneConsume);

// Define the mutex

osMutexId buffMutex;
osMutexDef(buffMutex);

osThreadId T_uart1;
osThreadId T_uart2;

#define CBUFFER_SIZE 8
#define DATA_SIZE 9

static int CBUFFER[CBUFFER_SIZE];
static int data[DATA_SIZE] = {1,2,3,4,5,6,7,8,9};
static int output [DATA_SIZE];
static int cbufferHead = 1;
static int cbufferTail = 1;
static int i = 0;
static int j = 0;

int main (void)
{
	osKernelInitialize ();

	doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
	doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), 1);	
	buffMutex = osMutexCreate(osMutex(buffMutex));
	T_uart1 = osThreadCreate(osThread(producer_thread), NULL);
	T_uart2 = osThreadCreate(osThread(consumer_thread), NULL);
	osKernelStart ();
}

void producer_thread (void const *argument)
{
	/*---
		wait or get event (control speed f data??)
		check if empty & full???
		----*/
  /*  if cbufferHead = (cbufferTail % (CBUFFER_SIZE-1)) + 1     //buffer full
    (

    )	*/
	for(;;)
	{
	osSemaphoreWait(doneConsume, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	CBUFFER[cbufferTail] = data[i];
	i++;
  	cbufferTail = (cbufferTail+1) % (CBUFFER_SIZE-1);
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneProduce);
	}
}

void consumer_thread(void const *argument)
{

/*    if cbufferHead = cbufferTail					// buffer empty
    (

    )	*/
	for(;;)
	{
	osSemaphoreWait(doneProduce, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	output[j] = CBUFFER[cbufferHead];
	j++;
	cbufferHead = (cbufferHead+1) % (CBUFFER_SIZE-1);
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneConsume);
	}
}


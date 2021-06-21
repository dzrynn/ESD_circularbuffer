#include "cmsis_os.h"
#include "stm32f10x.h"
#include "uart.h"

void producer_thread (void const *argument);
void consumer_thread(void const *argument);

// Define threads
osThreadDef(producer_thread, osPriorityNormal, 1, 0);
osThreadDef(consumer_thread, osPriorityNormal, 1, 0);

// Define the semaphores
osSemaphoreId doneProduce;      // Semaphore ID
osSemaphoreDef(doneProduce);    // Semaphore Definition
osSemaphoreId doneConsume;      // Semaphore ID
osSemaphoreDef(doneConsume);    // Semaphore Definition

// Define the mutex
osMutexId buffMutex;            // Mutex ID
osMutexDef(buffMutex);          // Mutex Definition

osThreadId T_consumer;
osThreadId T_producer;

#define BUFFER_SIZE 8

static int data[10] = {0,1,2,4,2,5,0,2,0,8};
unsigned char put = 0;
unsigned int output = 0;
unsigned char cbuffer [BUFFER_SIZE] = {0};
unsigned int head = 0;   // head = insert
unsigned int tail = 0;   // tail = remove


static int i=0;
static int j=0;

void queueProduce(unsigned char item)
{
	osSemaphoreWait(doneConsume, osWaitForever);     // ensure consumer thread is not dequeueing data
	osMutexWait(buffMutex, osWaitForever);			// secure mutex for critical section
	cbuffer[head] = data[item];
	head = (head + 1) % BUFFER_SIZE;
	osMutexRelease(buffMutex);						// done critical section, release mutex
	osSemaphoreRelease(doneProduce);				// indicate no more data to enqueue, consumer may run
}

unsigned char  queueConsume()
{
	unsigned int output =0XFF;
	osSemaphoreWait(doneProduce, osWaitForever);		// ensure producer thread is not enqueueing data
	osMutexWait(buffMutex, osWaitForever);				// secure mutex for critical section
	output = cbuffer[tail];
	tail = (tail + 1) % BUFFER_SIZE;
	osMutexRelease(buffMutex);							// done critical section, release mutex
	osSemaphoreRelease(doneConsume);					// indicate no more data to dequeue, producer may run
	return output;
}


int loop = 9;


void producer_thread (void const *argument){
	
	for(; i<loop; i++){									
		queueProduce(put);
		put++;
	}

}

void consumer_thread(void const *argument){
	
	for(; j<loop; j++){
		output = queueConsume();
		SendChar(output);
	}
}

int main (void)
{
	osKernelInitialize ();		  // initialize CMSIS-RTOS
	
	//USART1_Init();
	doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
	doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), 1);	
	buffMutex = osMutexCreate(osMutex(buffMutex));
  T_producer = osThreadCreate(osThread(producer_thread), NULL);
	T_consumer = osThreadCreate(osThread(consumer_thread), NULL);
	

	osKernelStart ();        // start thread execution 
}


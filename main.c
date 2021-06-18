#include "cmsis_os.h"
#include "stm32f10x.h"
#include "uart.h"

void producer_thread (void const *argument);
void consumer_thread(void const *argument);

// Define threads
osThreadDef(producer_thread, osPriorityNormal, 1, 0);
osThreadDef(consumer_thread, osPriorityNormal,1,0);

// Define the semaphores

osSemaphoreId doneProduce;      // Semaphore ID
osSemaphoreDef(doneProduce);    // Semaphore Definition
osSemaphoreId doneConsume;      // Semaphore ID
osSemaphoreDef(doneConsume);    // Semaphore Definition

// Define the mutex

osMutexId buffMutex;            // Mutex ID
osMutexDef(buffMutex);          // Mutex Definition

osThreadId T_uart1;
osThreadId T_uart2;

#define BUFFER_SIZE 8

unsigned char put = 0;
unsigned int output = 0;
unsigned char cbuffer [BUFFER_SIZE] = {0};
unsigned int head = 0;   // head = insert
unsigned int tail = 0;   // tail = remove


static int i=0;
static int j=0;

void enqueue(unsigned char item){
	osSemaphoreWait(doneConsume, osWaitForever);     // wait for the Semaphore
	osMutexWait(buffMutex, osWaitForever);
	cbuffer[head] = item;
	head = (head + 1) % BUFFER_SIZE;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneProduce);
}

unsigned char  dequeue()
{
	unsigned int a =0XFF;
	osSemaphoreWait(doneProduce, osWaitForever);
	osMutexWait(buffMutex, osWaitForever);
	a = cbuffer[tail];
	tail = (tail + 1) % BUFFER_SIZE;
	osMutexRelease(buffMutex);
	osSemaphoreRelease(doneConsume);
	return a;
}


int loop = 2;


void producer_thread (void const *argument){
	
	for(; i<loop; i++){
		enqueue(put++);
	}

}

void consumer_thread(void const *argument){
	
	for(; j<loop; j++){
		output=dequeue();
		SendChar(output);
	}
}

int main (void)
{
	osKernelInitialize ();		  // initialize CMSIS-RTOS
	
	USART1_Init();
	doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
	doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), BUFFER_SIZE);	
	buffMutex = osMutexCreate(osMutex(buffMutex));
	T_uart1 = osThreadCreate(osThread(producer_thread), NULL);
	T_uart2 = osThreadCreate(osThread(consumer_thread), NULL);
	

	osKernelStart ();        // start thread execution 
}


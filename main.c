#include "cmsis_os.h"
#include "stm32f10x.h"
#include "uart.h"

void producer_thread(void const *argument);
void consumer_thread_1(void const *argument);
void consumer_thread_2(void const *argument);
void uart_output_thread(void const *argument);


// Define threads
osThreadDef(producer_thread, osPriorityNormal, 1, 0);
osThreadDef(consumer_thread_1, osPriorityNormal, 1, 0);
osThreadDef(consumer_thread_2, osPriorityNormal, 1, 0);
osThreadDef(uart_output_thread, osPriorityNormal, 1, 0);


// Define the semaphores
osSemaphoreId doneProduce;      // Semaphore ID
osSemaphoreDef(doneProduce);    // Semaphore Definition
osSemaphoreId doneConsume;      // Semaphore ID
osSemaphoreDef(doneConsume);    // Semaphore Definition

// Define the mutex
osMutexId buffMutex;            // Mutex ID
osMutexDef(buffMutex);          // Mutex Definition



osThreadId T_producer;
osThreadId T_consumer_1;
osThreadId T_consumer_2;
osThreadId T_uart;

#define BUFFER_SIZE 5

static int data[10] = {9,1,2,4,2,5,0,2,0,8};
unsigned char put = 0;
unsigned int output = 0;
unsigned int output2 = 0;
unsigned char cbuffer [BUFFER_SIZE] = {0};
unsigned int head = 0;   // head = insert
unsigned int tail = 0;   // tail = remove

osMessageQId Q_buffer;
osMessageQDef (Q_buffer,0x16,unsigned char);
osEvent  result;


static int i=0;
static int j=0;
static int p=0;

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
	unsigned int q =0XFF;
	osSemaphoreWait(doneProduce, osWaitForever);		// ensure producer thread is not enqueueing data
	osMutexWait(buffMutex, osWaitForever);				// secure mutex for critical section
	q = cbuffer[tail];
	tail = (tail + 1) % BUFFER_SIZE;
	osMutexRelease(buffMutex);							// done critical section, release mutex
	osSemaphoreRelease(doneConsume);					// indicate no more data to dequeue, producer may run
	return q;
}


int loop = 11;


void producer_thread(void const *argument){
	
	for(; i<loop; i++){									
		queueProduce(put++);
	}
}


void consumer_thread_1(void const *argument){
	
	for(; j<loop; j++){
		output = queueConsume();
		osMessagePut(Q_buffer,output,osWaitForever);             //Place a value in the message queue
	}
}

void consumer_thread_2(void const *argument){
	
	for(; p<loop; p++){
		output2 = queueConsume();
		osMessagePut(Q_buffer,output2,osWaitForever);             //Place a value in the message queue
	}
}



void uart_output_thread(void const *argument){

	for(;;){
			result = osMessageGet(Q_buffer,osWaitForever);
			SendChar(result.value.v);
	
	}
	
}


int main (void)
{
	osKernelInitialize ();		  // initialize CMSIS-RTOS
	
	USART1_Init();
	
	doneProduce = osSemaphoreCreate(osSemaphore(doneProduce), 0);	
	doneConsume = osSemaphoreCreate(osSemaphore(doneConsume), BUFFER_SIZE);	
	buffMutex = osMutexCreate(osMutex(buffMutex));
	
	Q_buffer = osMessageCreate(osMessageQ(Q_buffer),NULL);
	
  	T_producer = osThreadCreate(osThread(producer_thread), NULL);      
	T_consumer_1 = osThreadCreate(osThread(consumer_thread_1), NULL);			
	T_consumer_2 = osThreadCreate(osThread(consumer_thread_2), NULL); 				
	T_uart = osThreadCreate(osThread(uart_output_thread), NULL); 	

	osKernelStart ();        // start thread execution 
}

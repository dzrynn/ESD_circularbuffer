#include "stm32f10x.h"
#include "cmsis_os.h"

// circular buffer 

// The hidden definition of  circular buffer structure
typedef struct {
  uint8_t * buffer;
	int head;
	int tail;
	const int maxlen;
}circular_buf_t;
 

// push data into the circuilar buffer
int circ_buf_push(circular_buf_t *c, uint8_t data)
{
    int next;

    next = c->head + 1;  // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;

    if (next == c->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;

    c->buffer[c->head] = data;  // Load data and then move
    c->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
}

// data taken out from the buffer
int circ_bbuf_pop(circular_buf_t *c, uint8_t *data)
{
    int next;

    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail];  // Read data and then move
    c->tail = next;              // tail to next offset.
    return 0;  // return success to indicate successful push.
}
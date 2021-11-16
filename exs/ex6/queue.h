#ifndef _QUEUE_H
#define _QUEUE_H

// A linked list (LL) node to store a queue entry 
typedef struct QNode {
    void *key;
    struct QNode *next;
} QNode;

// The queue, front stores the front node of LL and rear stores the 
// last node of LL 
typedef struct Queue {
    struct QNode *front, *rear;
} Queue;


Queue *createQueue();

//q - the queue, k- the key (element)
int enQueue(Queue *q, void *k);

void deQueue(Queue *q);

// retutn 1 if empty and zero if not empty
int isEmpty(Queue *q);

void destroyQueue(Queue *q);

#endif

#ifndef CONTQUEUE_H
#define CONTQUEUE_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 3

// Структура узел
typedef struct tdNode {
    void* data;          // Данные
    struct tdNode* next; // Следующий узел
} tdNode;

// Структура очереди
typedef struct {
    tdNode* head; // Первый элемент
    tdNode* tail; // Последний элемент
    int size;     // Размер очереди
} tdQueue;

// Структура итератора
typedef struct {
    tdQueue* queue;
    tdNode* current;
} QueueIterator;

// Прототипы

tdQueue* queue_create(void);
void queue_enqueue(tdQueue* q, void* d);
void queue_print(tdQueue* q);
void* queue_dequeue(tdQueue* q);
bool queue_is_empty(tdQueue* q);
int queue_size(tdQueue* q);
void* queue_peek(tdQueue* q);
void* queue_peek_last(tdQueue* q);
void queue_clear(tdQueue* q);
void queue_fenqueue(tdQueue* q, void* d);
tdQueue* queue_copy(tdQueue* orig);
tdQueue* queue_merge(tdQueue* q1, tdQueue* q2);

void queue_free(tdQueue* q);

QueueIterator* queue_iterator_create(tdQueue* q, int index);
void* queue_iterator_next(QueueIterator* it);
bool queue_iterator_check_queue(QueueIterator* it, tdQueue* q);
bool queue_iterator_is_equal(QueueIterator* it1, QueueIterator* it2);

#endif

#include "contqueue.h"

// Создание пустой очереди
tdQueue* queue_create(void)
{
    tdQueue* q = malloc(sizeof(tdQueue));
    if (q == NULL) {
        puts("Ошибка выделения памяти");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

// Добавление элемента в конец очереди
void queue_enqueue(tdQueue* q, void* d)
{
    if (q->size >= MAX_QUEUE_SIZE) {
        puts("Ошибка очередь заполнена");
        return;
    }
    tdNode* new_node = calloc(1, sizeof(tdNode));
    if (new_node == NULL) {
        puts("Ошибка выделения памяти");
        return;
    }
    new_node->data = d;
    new_node->next = NULL;
    if (q->tail == NULL) {
        q->head = new_node;
        q->tail = new_node;
    } else {
        q->tail->next = new_node;
        q->tail = new_node;
    }
    q->size++;
    // printf("%p добавлен в очередь %p\n", d, q);
}

// Вывод очереди
void queue_print(tdQueue* q)
{
    printf("Очередь %p\n", q);
    if (queue_is_empty(q)) {
        puts("Очередь пуста");
        return;
    }
    tdNode* curr = q->head;
    while (curr) {
        printf("%p\n", curr->data);
        curr = curr->next;
    }
}

// Вывод matrix2d в очереди
void queue_print_matrix2d(tdQueue* q)
{
    int i = 0;

    QueueIterator* it = queue_iterator_create(q, 0);
    if (it == NULL)
        return;
    while (it->current != NULL) {
        printf("%d\n", i++);
        show_matrix2d(queue_iterator_next(it));
    }
    free(it);
}

// Удаление элемента из начала очереди
void* queue_dequeue(tdQueue* q)
{
    if (queue_is_empty(q)) {
        printf("Очередь пуста\n");
        return NULL;
    }
    tdNode* temp = q->head;
    void* data = temp->data;
    q->head = q->head->next;
    free(temp);
    q->size--;
    return data;
}

// Проверка на пустоту
bool queue_is_empty(tdQueue* q)
{
    return q->head == NULL;
}

// Получение размера очереди
int queue_size(tdQueue* q)
{
    return q->size;
}

// Просмотр первого элемента
void* queue_peek(tdQueue* q)
{
    if (queue_is_empty(q)) {
        printf("Очередь пуста\n");
        return NULL;
    }
    return q->head->data;
}

// Просмотр последнего элемента
void* queue_peek_last(tdQueue* q)
{
    if (queue_is_empty(q)) {
        printf("Очередь пуста\n");
        return NULL;
    }
    return q->tail->data;
}

// Очистка очереди
void queue_clear(tdQueue* q)
{
    tdNode* curr = q->head;
    while (curr) {
        tdNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

// Добавление элемента в начало очереди
void queue_fenqueue(tdQueue* q, void* d)
{
    if (q->size >= MAX_QUEUE_SIZE) {
        puts("Ошибка очередь заполнена");
        return;
    }
    tdNode* new_node = calloc(1, sizeof(tdNode));
    if (new_node == NULL) {
        perror("Ошибка выделения памяти\n");
        return;
    }
    new_node->data = d;
    new_node->next = q->head;
    q->head = new_node;
    if (q->tail == NULL) {
        q->tail = new_node;
    }
    q->size++;
    // printf("%p добавлен в очередь %p\n", d, q);
}

// Освобождение памяти
void queue_free(tdQueue* q)
{
    tdNode* curr = q->head;
    while (curr) {
        tdNode* temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(q);
}

// Освобождение памяти, включая matrix2d
void queue_free_with_matrix2d(tdQueue* q)
{
    tdNode* curr = q->head;
    while (curr) {
        tdNode* temp = curr;
        curr = curr->next;
        destroy_matrix2d(temp->data, true);
        free(temp);
    }
    free(q);
}

// Копирование очереди
tdQueue* queue_copy(tdQueue* orig)
{
    tdQueue* copy = queue_create();

    tdNode* curr = orig->head;
    while (curr) {
        void* d = curr->data;
        Matrix2d* m_copy = calloc(1, sizeof(Matrix2d));
        matrix2d_copy_constructor(d, m_copy);
        queue_enqueue(copy, m_copy);
        curr = curr->next;
    }

    return copy;
}

// Слияние двух стеков
tdQueue* queue_merge(tdQueue* q1, tdQueue* q2)
{
    tdQueue* merge = queue_create();

    tdNode* curr = q1->head;
    while (curr) {
        void* d = curr->data;
        Matrix2d* m_copy = calloc(1, sizeof(Matrix2d));
        matrix2d_copy_constructor(d, m_copy);
        queue_enqueue(merge, m_copy);
        curr = curr->next;
    }

    curr = q2->head;
    while (curr) {
        void* d = curr->data;
        Matrix2d* m_copy = calloc(1, sizeof(Matrix2d));
        matrix2d_copy_constructor(d, m_copy);
        queue_enqueue(merge, m_copy);
        curr = curr->next;
    }

    return merge;
}

// Создание итератора очереди
QueueIterator* queue_iterator_create(tdQueue* q, int index)
{
    if ((index < 0) || (index > q->size)) {
        puts("Ошибка: недопустимый индекс");
        return NULL;
    }
    QueueIterator* it = calloc(1, sizeof(QueueIterator));
    if (it == NULL) {
        puts("Ошибка выделения памяти");
        return NULL;
    }

    it->queue = q;
    it->current = q->head;
    while (index > 0) {
        it->current = it->current->next;
        index--;
    }

    return it;
}

// Получение данных итератором
void* queue_iterator_next(QueueIterator* it)
{
    if (it->current == NULL) {
        // printf("Итератор %p дошёл до конца\n", it);
        return NULL;
    }

    void* temp = it->current->data;
    it->current = it->current->next;

    return temp;
}

// Проверка очереди итератора
bool queue_iterator_check_queue(QueueIterator* it, tdQueue* q)
{
    return it->queue == q;
}

// Проверка на равенство итераторов
bool queue_iterator_is_equal(QueueIterator* it1, QueueIterator* it2)
{
    return (it1->queue == it2->queue) && (it1->current == it2->current);
}

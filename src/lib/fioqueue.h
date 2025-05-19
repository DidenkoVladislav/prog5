#ifndef FIOQUEUE_H
#define FIOQUEUE_H

#include "contqueue.h"
#include "matrix2d.h"
#include <stdio.h>
#include <stdlib.h>

#define ELEMENTS 2
#define TEXT_MODE 0
#define BINARY_MODE 1

// Прототипы

Matrix2d** rand_gen_matrix2d(size_t n);
tdQueue* rand_gen_matrix2d_in_queue(size_t n);
int save_matrix2d_to_file(const char* filename, Matrix2d* m, int mode);
Matrix2d* load_matrix2d_from_file(const char* filename, int mode);
int save_queue_to_file(const char* filename, tdQueue* q, int mode);
tdQueue* load_queue_from_file(const char* filename, int mode);
Matrix2d* get_matrix2d_from_text_file(const char* filename, int index);
int get_matrix2d_from_binary_file(
        const char* filename, int index, Matrix2d* result);

#endif

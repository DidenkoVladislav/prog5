#ifndef __MATRIX2D_H__
#define __MATRIX2D_H__

#include "bitrgbled.h"
#include <math.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5

// Тип данных элементов матрицы
typedef enum { union_var_i, union_var_d } TYPE_VAR;

typedef union {
    int var_i;
    double var_d;
} TAG_VAR;

// Структура - матрица
typedef struct {
    alignas(16) size_t rows;
    alignas(16) size_t cols;
    alignas(8) TYPE_VAR type;
    alignas(8) TAG_VAR** arr;
    alignas(16) ulg settings;
    alignas(8) BITrgbled* bitrgbled;
} Matrix2d;

Matrix2d* matrix2d_constructor(int rows, int cols, int type);
Matrix2d* matrix2d_default_constructor(void);
void matrix2d_copy_constructor(Matrix2d* m, Matrix2d* n);

void setter_matrix2d(
        Matrix2d* m, int is_edit, size_t rows, size_t cols, TYPE_VAR type);
void set_size_matrix2d(Matrix2d* m, size_t rows, size_t cols);
void set_type_matrix2d(Matrix2d* m, TYPE_VAR type);

TAG_VAR** create_arr(size_t rows, size_t cols);
void fill_matrix2d_array(Matrix2d* m);
void destroy_arr(Matrix2d* m);

int is_equal(Matrix2d* a, Matrix2d* b);
int is_not_equal(Matrix2d* a, Matrix2d* b);
double get_matrix2d_sum(Matrix2d* m);
int is_greater(Matrix2d* a, Matrix2d* b);
int is_less(Matrix2d* a, Matrix2d* b);
int is_greater_or_equal(Matrix2d* a, Matrix2d* b);
int is_less_or_equal(Matrix2d* a, Matrix2d* b);

void increment_matrix2d(Matrix2d* m);
void decrement_matrix2d(Matrix2d* m);

void get_matrix2d_col(Matrix2d* m, size_t col);
void get_matrix2d_row(Matrix2d* m, size_t row);

void show_matrix2d(Matrix2d* m);

Matrix2d* random_matrix2d(size_t rows, size_t cols, TYPE_VAR type);

Matrix2d* matrix_transpose(Matrix2d* m);

double find_determ(Matrix2d* m);
Matrix2d* calc_inverse_matrix(Matrix2d* m);
double GaussDet(double** m, int n);
double**
getminor(TAG_VAR** arr, TYPE_VAR type, int rows, int cols, int row, int col);

void destroy_matrix2d(Matrix2d* m, bool is_quiet);

#endif

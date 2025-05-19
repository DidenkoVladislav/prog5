#include <lib/fioqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    tdQueue* q1 = rand_gen_matrix2d_in_queue(2);
    puts("Сгенерированная очередь q1");
    queue_print_matrix2d(q1);
    puts("\nСохраним очередь q1 в текстовый файл");
    save_queue_to_file("q1.txt", q1, TEXT_MODE);

    tdQueue* q2 = load_queue_from_file("q1.txt", TEXT_MODE);
    puts("\nq2 - очередь загруженная из текстового файла");
    queue_print_matrix2d(q2);

    puts("\nВозьмём первый элемент из очереди q2 в mat1");
    Matrix2d* mat1 = queue_dequeue(q2);
    show_matrix2d(mat1);

    puts("\nq2");
    queue_print_matrix2d(q2);

    puts("\nСохраним очередь q2 в бинарный файл");
    save_queue_to_file("q2.bin", q2, BINARY_MODE);

    tdQueue* q3 = load_queue_from_file("q2.bin", BINARY_MODE);
    puts("\nq3 - очередь загруженная из бинарного файла");
    queue_print_matrix2d(q3);

    puts("\nСкопируем q1 в q4");
    tdQueue* q4 = queue_copy(q1);
    puts("q1");
    queue_print_matrix2d(q1);
    puts("q4");
    queue_print_matrix2d(q4);

    puts("\nУберём первый элемент из q4");
    destroy_matrix2d(queue_dequeue(q4), true);
    puts("q1");
    queue_print_matrix2d(q1);
    puts("q4");
    queue_print_matrix2d(q4);

    puts("\nСольём q3 и q1 в q5");
    tdQueue* q5 = queue_merge(q3, q1);
    queue_print_matrix2d(q5);

    puts("\nПоработаем с mat1");
    show_matrix2d(mat1);
    printf("Сумма элементов mat1: %f\n", get_matrix2d_sum(mat1));

    puts("\nПолучим транспонированную матрицу trans (от mat1)");
    Matrix2d* trans = matrix_transpose(mat1);
    show_matrix2d(trans);

    puts("\nСоздадим матрицу 3x3 - mat3");
    Matrix2d* mat3 = random_matrix2d(3, 3, union_var_d);
    show_matrix2d(mat3);

    puts("\nНайдём обратную матрицу mat3_invers");
    Matrix2d* mat3_invers = calc_inverse_matrix(mat3);
    show_matrix2d(mat3_invers);

    queue_free_with_matrix2d(q1);
    q1 = NULL;
    queue_free_with_matrix2d(q2);
    q2 = NULL;
    destroy_matrix2d(mat1, true);
    mat1 = NULL;
    destroy_matrix2d(trans, true);
    trans = NULL;
    destroy_matrix2d(mat3, true);
    mat3 = NULL;
    destroy_matrix2d(mat3_invers, true);
    mat3_invers = NULL;
    queue_free_with_matrix2d(q3);
    q3 = NULL;
    queue_free_with_matrix2d(q4);
    q4 = NULL;
    queue_free_with_matrix2d(q5);
    q5 = NULL;

    return 0;
}

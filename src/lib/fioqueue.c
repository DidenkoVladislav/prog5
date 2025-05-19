#include "fioqueue.h"

Matrix2d** rand_gen_matrix2d(size_t n)
{
    Matrix2d** matrices = (Matrix2d**)calloc(n, sizeof(Matrix2d*));

    for (size_t i = 0; i < n; i++) {
        int rows = rand() % MAX_SIZE + 1;
        int cols = rand() % MAX_SIZE + 1;
        TYPE_VAR type = rand() % 2;

        matrices[i] = random_matrix2d(rows, cols, type);
    }

    return matrices;
}

tdQueue* rand_gen_matrix2d_in_queue(size_t n)
{
    tdQueue* q = queue_create();
    if (q == NULL)
        return q;

    for (size_t i = 0; i < n; i++) {
        int rows = rand() % MAX_SIZE + 1;
        int cols = rand() % MAX_SIZE + 1;
        TYPE_VAR type = rand() % 2;

        Matrix2d* m = random_matrix2d(rows, cols, type);

        queue_enqueue(q, m);
    }

    return q;
}

static void save_matrix2d_text(FILE* fp, Matrix2d* m)
{
    fprintf(fp, "%zu %zu %d ", m->rows, m->cols, m->type);

    for (size_t i = 0, j; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->type == union_var_i) {
                fprintf(fp, "%d ", m->arr[i][j].var_i);
            } else {
                fprintf(fp, "%lf ", m->arr[i][j].var_d);
            }
        }
    }

    fprintf(fp, "%ld\n", m->settings);
}

static void save_matrix2d_binary(FILE* fp, Matrix2d* m)
{
    fwrite(&m->rows, sizeof(size_t), 1, fp);
    fwrite(&m->cols, sizeof(size_t), 1, fp);
    fwrite(&m->type, sizeof(int), 1, fp);

    for (size_t i = 0, j; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->type == union_var_i) {
                fwrite(&m->arr[i][j].var_i, sizeof(int), 1, fp);
            } else {
                fwrite(&m->arr[i][j].var_d, sizeof(double), 1, fp);
            }
        }
    }

    fwrite(&m->settings, sizeof(ulg), 1, fp);
}

// Сохранение matrix2d в файл
int save_matrix2d_to_file(const char* filename, Matrix2d* m, int mode)
{
    FILE* fp = (mode == TEXT_MODE) ? (fopen(filename, "w"))
                                   : (fopen(filename, "wb"));
    if (fp == NULL) {
        printf("Ошибка записи файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (mode == TEXT_MODE) {
        save_matrix2d_text(fp, m);
    } else {
        save_matrix2d_binary(fp, m);
    }

    fclose(fp);

    return 1;
}

static Matrix2d* load_matrix2d_from_text(FILE* fp)
{
    size_t rows, cols;
    int type;

    if (fscanf(fp, "%zu %zu %d", &rows, &cols, &type) != 3) {
        return NULL;
    }

    Matrix2d* m = calloc(1, sizeof(Matrix2d));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    m->type = type;
    m->arr = create_arr(rows, cols);

    for (size_t i = 0, j; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->type == union_var_i) {
                if (fscanf(fp, "%d", &m->arr[i][j].var_i) != 1) {
                    destroy_matrix2d(m, true);
                    return NULL;
                }
            } else {
                if (fscanf(fp, "%lf", &m->arr[i][j].var_d) != 1) {
                    destroy_matrix2d(m, true);
                    return NULL;
                }
            }
        }
    }

    if (fscanf(fp, "%ld", &m->settings) != 1) {
        destroy_matrix2d(m, true);
        return NULL;
    }

    m->bitrgbled = calloc(1, sizeof(BITrgbled));
    if (m->bitrgbled == NULL) {
        destroy_matrix2d(m, true);
        return NULL;
    }
    bitrgbled_struct_from_bitrgbled(&m->settings, m->bitrgbled);

    return m;
}

static Matrix2d* load_matrix2d_from_binary(FILE* fp)
{
    size_t rows, cols;
    int type;

    if (fread(&rows, sizeof(size_t), 1, fp) != 1) {
        return NULL;
    }

    if (fread(&cols, sizeof(size_t), 1, fp) != 1) {
        return NULL;
    }

    if (fread(&type, sizeof(int), 1, fp) != 1) {
        return NULL;
    }

    Matrix2d* m = calloc(1, sizeof(Matrix2d));
    if (m == NULL) {
        return NULL;
    }

    m->rows = rows;
    m->cols = cols;
    m->type = type;
    m->arr = create_arr(rows, cols);

    for (size_t i = 0, j; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            if (m->type == union_var_i) {
                if (fread(&m->arr[i][j].var_i, sizeof(int), 1, fp) != 1) {
                    destroy_matrix2d(m, true);
                    return NULL;
                }
            } else {
                if (fread(&m->arr[i][j].var_d, sizeof(double), 1, fp) != 1) {
                    destroy_matrix2d(m, true);
                    return NULL;
                }
            }
        }
    }

    if (fread(&m->settings, sizeof(ulg), 1, fp) != 1) {
        destroy_matrix2d(m, true);
        return NULL;
    }

    m->bitrgbled = calloc(1, sizeof(BITrgbled));
    if (m->bitrgbled == NULL) {
        destroy_matrix2d(m, true);
        return NULL;
    }
    bitrgbled_struct_from_bitrgbled(&m->settings, m->bitrgbled);

    return m;
}

// Загрузка matrix2d из файла
Matrix2d* load_matrix2d_from_file(const char* filename, int mode)
{
    FILE* fp = (mode == TEXT_MODE) ? (fopen(filename, "r"))
                                   : (fopen(filename, "rb"));
    if (fp == NULL) {
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    Matrix2d* m = (mode == TEXT_MODE) ? (load_matrix2d_from_text(fp))
                                      : (load_matrix2d_from_binary(fp));

    fclose(fp);

    return m;
}

// Сохранение queue в файл
int save_queue_to_file(const char* filename, tdQueue* q, int mode)
{
    FILE* fp = (mode == TEXT_MODE) ? (fopen(filename, "w"))
                                   : (fopen(filename, "wb"));
    if (fp == NULL) {
        printf("Ошибка записи файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    QueueIterator* it = queue_iterator_create(q, 0);
    Matrix2d* m;

    if (mode == TEXT_MODE) {
        fprintf(fp, "%d\n", queue_size(q));
        while ((m = queue_iterator_next(it)) != NULL) {
            save_matrix2d_text(fp, m);
        }
    } else {
        int size = queue_size(q);
        fwrite(&size, sizeof(int), 1, fp);
        while ((m = queue_iterator_next(it)) != NULL) {
            save_matrix2d_binary(fp, m);
        }
    }

    free(it);
    fclose(fp);

    return 1;
}

// Загрузка queue из файла
tdQueue* load_queue_from_file(const char* filename, int mode)
{
    FILE* fp = (mode == TEXT_MODE) ? (fopen(filename, "r"))
                                   : (fopen(filename, "rb"));
    if (fp == NULL) {
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    tdQueue* q = queue_create();

    int size;
    if (mode == TEXT_MODE) {
        if (fscanf(fp, "%d\n", &size) != 1) {
            queue_free(q);
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }
    } else {
        if (fread(&size, sizeof(int), 1, fp) != 1) {
            queue_free(q);
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < size; i++) {
        Matrix2d* m = (mode == TEXT_MODE) ? (load_matrix2d_from_text(fp))
                                          : (load_matrix2d_from_binary(fp));
        if (m == NULL) {
            queue_free(q);
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }
        queue_enqueue(q, m);
    }

    fclose(fp);

    return q;
}

// matrix2d из текстового файла по индексу
Matrix2d* get_matrix2d_from_text_file(const char* filename, int index)
{
    if (index < 0) {
        printf("Ошибка индекса %d: ожидалось index >= 0\n", index);
        exit(EXIT_FAILURE);
    }
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fclose(fp);
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int size;
    if ((fscanf(fp, "%d\n", &size) != 1)) {
        fclose(fp);
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }
    if ((index >= size)) {
        fclose(fp);
        printf("Ошибка индекса %d: ожидалось index < size\n", index);
        exit(EXIT_FAILURE);
    }

    Matrix2d* result = NULL;
    for (int i = 0; i <= index; i++) {
        if (result) {
            destroy_matrix2d(result, true);
        }
        result = load_matrix2d_from_text(fp);
        if (result == NULL) {
            break;
        }
    }

    fclose(fp);

    return result;
}

// matrix2d из бинарного файла по индексу
int get_matrix2d_from_binary_file(
        const char* filename, int index, Matrix2d* result)
{
    if (index < 0) {
        printf("Ошибка индекса %d: ожидалось index >= 0\n", index);
        exit(EXIT_FAILURE);
    }
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        fclose(fp);
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int size;
    if ((fread(&size, sizeof(int), 1, fp) != 1)) {
        fclose(fp);
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }
    if ((index >= size)) {
        fclose(fp);
        printf("Ошибка индекса %d: ожидалось index < size\n", index);
        exit(EXIT_FAILURE);
    }

    long pos = sizeof(int);
    for (int i = 0; i < index; i++) {
        size_t rows, cols;
        int type;

        if (fread(&rows, sizeof(size_t), 1, fp) != 1) {
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }

        if (fread(&cols, sizeof(size_t), 1, fp) != 1) {
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }

        if (fread(&type, sizeof(int), 1, fp) != 1) {
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }

        size_t elements_size
                = ((type == union_var_i) ? (sizeof(int)) : (sizeof(double)))
                * rows * cols;
        pos += 2 * sizeof(size_t) + sizeof(int) + elements_size + sizeof(ulg);

        if (fseek(fp, elements_size + sizeof(ulg), SEEK_CUR) != 0) {
            fclose(fp);
            printf("Ошибка считывания файла %s\n", filename);
            exit(EXIT_FAILURE);
        }
    }

    if (fseek(fp, pos, SEEK_SET) != 0) {
        fclose(fp);
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    Matrix2d* m = load_matrix2d_from_binary(fp);
    fclose(fp);

    if (m == NULL) {
        printf("Ошибка считывания файла %s\n", filename);
        exit(EXIT_FAILURE);
    }

    matrix2d_copy_constructor(m, result);
    destroy_matrix2d(m, true);

    return 1;
}

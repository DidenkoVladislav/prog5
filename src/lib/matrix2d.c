#include "matrix2d.h"

static void mem_check(void* p)
{
    if (p == NULL) {
        free(p);
        puts("Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }
}

// Заполнение массива
void fill_matrix2d_array(Matrix2d* m)
{
    mem_check(m);
    printf("Введите %ld элемента: ", m->cols * m->rows);
    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < m->cols; ++i) {
            for (j = 0; j < m->rows; ++j) {
                if (scanf("%d", &m->arr[i][j].var_i) != 1) {
                    puts("Некорректный ввод.");
                    exit(EXIT_FAILURE);
                }
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                if (scanf("%lf", &m->arr[i][j].var_d) != 1) {
                    puts("Некорректный ввод.");
                    exit(EXIT_FAILURE);
                }
            }
        }
        break;
    }
}

// Конструктор
Matrix2d* matrix2d_constructor(int rows, int cols, int type)
{
    Matrix2d* m = calloc(1, sizeof(Matrix2d));
    mem_check(m);
    m->type = type;
    m->cols = cols;
    m->rows = rows;

    m->arr = create_arr(rows, cols);

    fill_matrix2d_array(m);

    m->settings = 0;

    m->bitrgbled = calloc(1, sizeof(BITrgbled));
    if (m->bitrgbled == NULL)
        exit(EXIT_FAILURE);

    return m;
}

void set_size_matrix2d(Matrix2d* m, size_t rows, size_t cols)
{
    mem_check(m);
    if ((m->rows != rows) || (m->cols != cols)) {
        if (m->arr != NULL)
            destroy_arr(m);
        m->rows = rows;
        m->cols = cols;
        m->arr = create_arr(rows, cols);
    }
}

void set_type_matrix2d(Matrix2d* m, TYPE_VAR type)
{
    mem_check(m);
    m->type = type;
}

// Сеттер
void setter_matrix2d(
        Matrix2d* m, int is_edit, size_t rows, size_t cols, TYPE_VAR type)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }

    set_size_matrix2d(m, rows, cols);

    set_type_matrix2d(m, type);

    if (is_edit) {
        fill_matrix2d_array(m);
    }
}

// Конструктор "по умолчанию"
Matrix2d* matrix2d_default_constructor(void)
{
    Matrix2d* m = calloc(1, sizeof(Matrix2d));
    mem_check(m);

    setter_matrix2d(m, 0, 2, 2, union_var_i);
    m->arr[0][0].var_i = 1;
    m->arr[0][1].var_i = 0;
    m->arr[1][0].var_i = 0;
    m->arr[1][1].var_i = 1;

    m->settings = 0;

    m->bitrgbled = calloc(1, sizeof(BITrgbled));
    if (m->bitrgbled == NULL)
        exit(EXIT_FAILURE);

    return m;
}

// Конструктор "копирования"
void matrix2d_copy_constructor(Matrix2d* m, Matrix2d* n)
{
    if (m == NULL || n == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    if ((m->cols != n->cols) || (m->rows != n->rows)) {
        if (n->arr != NULL)
            destroy_arr(n);
        n->cols = m->cols;
        n->rows = m->rows;
        n->arr = create_arr(m->rows, m->cols);
    }

    n->type = m->type;
    switch (n->type) {
    case union_var_i:
        for (size_t i = 0, j; i < n->rows; ++i) {
            for (j = 0; j < n->cols; ++j) {
                n->arr[i][j].var_i = m->arr[i][j].var_i;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                n->arr[i][j].var_d = m->arr[i][j].var_d;
            }
        }
        break;
    }

    n->settings = m->settings;

    n->bitrgbled->red = m->bitrgbled->red;
    n->bitrgbled->green = m->bitrgbled->green;
    n->bitrgbled->blue = m->bitrgbled->blue;
    n->bitrgbled->brightness = m->bitrgbled->brightness;
    n->bitrgbled->colortemp = m->bitrgbled->colortemp;
    n->bitrgbled->controltype = m->bitrgbled->controltype;
    n->bitrgbled->mode = m->bitrgbled->mode;
}

// Создание массива
TAG_VAR** create_arr(size_t rows, size_t cols)
{
    TAG_VAR** array2D = (TAG_VAR**)calloc(rows, sizeof(TAG_VAR*));
    if (array2D == NULL)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i < rows; i++) {
        array2D[i] = (TAG_VAR*)calloc(cols, sizeof(TAG_VAR));

        if (array2D[i] == NULL) {
            for (size_t j = 0; j < i; j++)
                free(array2D[j]);
            free(array2D);
            exit(EXIT_FAILURE);
        }
    }

    return array2D;
}

// Очистка массива
void destroy_arr(Matrix2d* m)
{
    for (size_t i = 0; i < m->rows; i++)
        free(m->arr[i]);
    free(m->arr);
    m->arr = NULL;
}

// Очистка структуры matrix2d
void destroy_matrix2d(Matrix2d* m, bool is_quiet)
{
    if (!is_quiet) {
        printf("%p - освобождается.\n", m->arr);
        printf("%p - освобождается.\n", m->bitrgbled);
        printf("%p - освобождается.\n", m);
    }
    destroy_arr(m);
    destroy_struct_bitrgbled(m->bitrgbled);
    free(m);
    m = NULL;
}

// Если размеры одинаковы, то сверяются поэлементно. Иначе 0.
int is_equal(Matrix2d* a, Matrix2d* b)
{
    if (a == NULL || b == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    if (a->type != b->type)
        return 0;
    switch (a->type) {
    case union_var_i:
        for (size_t i = 0, j; i < a->rows; ++i) {
            for (j = 0; j < a->cols; ++j) {
                if (a->arr[i][j].var_i != b->arr[i][j].var_i)
                    return 0;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < a->rows; ++i) {
            for (j = 0; j < a->cols; ++j) {
                if (a->arr[i][j].var_d != b->arr[i][j].var_d)
                    return 0;
            }
        }
        break;
    }

    return 1;
}

// Если размеры одинаковы, то сверяются поэлементно. Иначе 0.
int is_not_equal(Matrix2d* a, Matrix2d* b)
{
    return !is_equal(a, b);
}

// Получение суммы элементов
double get_matrix2d_sum(Matrix2d* m)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    double result = 0;

    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                result += (double)m->arr[i][j].var_i;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                result += m->arr[i][j].var_d;
            }
        }
        break;
    }
    return result;
}

// Сравнение по сумме
int is_greater(Matrix2d* a, Matrix2d* b)
{
    if (a == NULL || b == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    return get_matrix2d_sum(a) > get_matrix2d_sum(b);
}

// Сравнение по сумме
int is_less(Matrix2d* a, Matrix2d* b)
{
    if (a == NULL || b == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    return get_matrix2d_sum(a) < get_matrix2d_sum(b);
}

// Сравнение по сумме
int is_greater_or_equal(Matrix2d* a, Matrix2d* b)
{
    if (a == NULL || b == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    return get_matrix2d_sum(a) >= get_matrix2d_sum(b);
}

// Сравнение по сумме
int is_less_or_equal(Matrix2d* a, Matrix2d* b)
{
    if (a == NULL || b == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    return get_matrix2d_sum(a) <= get_matrix2d_sum(b);
}

// Вывод матрицы
void show_matrix2d(Matrix2d* m)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }

    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                printf("%d ", m->arr[i][j].var_i);
            }
            puts("");
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                printf("%.2lf ", m->arr[i][j].var_d);
            }
            puts("");
        }
        break;
    }
}

// Инкремент элементов
void increment_matrix2d(Matrix2d* m)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < m->cols; ++i) {
            for (j = 0; j < m->rows; ++j) {
                m->arr[i][j].var_i++;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->cols; ++i) {
            for (j = 0; j < m->rows; ++j) {
                m->arr[i][j].var_d++;
            }
        }
        break;
    }
}

// Декремент элементов
void decrement_matrix2d(Matrix2d* m)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < m->cols; ++i) {
            for (j = 0; j < m->rows; ++j) {
                m->arr[i][j].var_i--;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->cols; ++i) {
            for (j = 0; j < m->rows; ++j) {
                m->arr[i][j].var_d--;
            }
        }
        break;
    }
}

// Создание структуры со случайной матрицей
Matrix2d* random_matrix2d(size_t rows, size_t cols, TYPE_VAR type)
{
    Matrix2d* m = calloc(1, sizeof(Matrix2d));
    mem_check(m);
    setter_matrix2d(m, 0, rows, cols, type);

    if (type == union_var_i) {
        for (size_t i = 0, j; i < rows; ++i) {
            for (j = 0; j < cols; ++j) {
                m->arr[i][j].var_i = rand() % 46 - 20;
            }
        }
    } else {
        for (size_t i = 0, j; i < rows; ++i) {
            for (j = 0; j < cols; ++j) {
                m->arr[i][j].var_d = (double)(rand() % 46 - 20);
            }
        }
    }

    m->settings = rand_gen_bitrgbled();

    m->bitrgbled = calloc(1, sizeof(BITrgbled));
    if (m->bitrgbled == NULL)
        exit(EXIT_FAILURE);
    bitrgbled_struct_from_bitrgbled(&m->settings, m->bitrgbled);

    return m;
}

// Вывод столбца матрицы
void get_matrix2d_col(Matrix2d* m, size_t col)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    if (col > m->cols) {
        puts("Некорректный ввод.");
        exit(EXIT_FAILURE);
    }

    switch (m->type) {
    case union_var_i:
        for (size_t i = 0; i < m->rows; ++i) {
            printf("%d\n", m->arr[i][col].var_i);
        }
        break;

    case union_var_d:
        for (size_t i = 0; i < m->rows; ++i) {
            printf("%.2lf\n", m->arr[i][col].var_d);
        }
        break;
    }
}

// Вывод строки матрицы
void get_matrix2d_row(Matrix2d* m, size_t row)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    if (row > m->rows) {
        puts("Некорректный ввод.");
        exit(EXIT_FAILURE);
    }

    switch (m->type) {
    case union_var_i:
        for (size_t i = 0; i < m->cols; ++i) {
            printf("%d ", m->arr[row][i].var_i);
        }
        puts("");
        break;

    case union_var_d:
        for (size_t i = 0; i < m->cols; ++i) {
            printf("%.2lf ", m->arr[row][i].var_d);
        }
        puts("");
        break;
    }
}

// Получение структуры с транспонированной матрицой
Matrix2d* matrix_transpose(Matrix2d* m)
{
    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    Matrix2d* tran = calloc(1, sizeof(Matrix2d));
    mem_check(tran);
    tran->cols = m->rows;
    tran->rows = m->cols;
    tran->arr = create_arr(tran->cols, tran->rows);

    tran->type = m->type;
    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < tran->cols; ++i) {
            for (j = 0; j < tran->rows; ++j) {
                tran->arr[i][j].var_i = m->arr[j][i].var_i;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < tran->cols; ++i) {
            for (j = 0; j < tran->rows; ++j) {
                tran->arr[i][j].var_d = m->arr[j][i].var_d;
            }
        }
        break;
    }

    return tran;
}

// Детерминант методом Гаусса
double GaussDet(double** m, int n)
{
    double det = 1;
    for (int i = 0; i < n; ++i) {
        double mx = fabs(m[i][i]);
        int idx = i;
        for (int j = i + 1; j < n; ++j)
            if (mx < fabs(m[i][j]))
                mx = fabs(m[i][idx = j]);
        if (idx != i) {
            for (int j = i; j < n; ++j) {
                double t = m[j][i];
                m[j][i] = m[j][idx];
                m[j][idx] = t;
            }
            det = -det;
        }
        for (int k = i + 1; k < n; ++k) {
            double t = m[k][i] / m[i][i];

            for (int j = i; j < n; ++j)
                m[k][j] -= m[i][j] * t;
        }
    }
    for (int i = 0; i < n; ++i)
        det *= m[i][i];

    return det;
}

// Получение детерминанта
double find_determ(Matrix2d* m)
{
    double det;

    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }

    if (m->cols != m->rows) {
        puts("Матрица неквадратная.");
        exit(EXIT_FAILURE);
    }

    double** c_arr = (double**)calloc(m->rows, sizeof(double*));
    if (c_arr == NULL)
        exit(EXIT_FAILURE);
    for (size_t i = 0, j; i < m->rows; i++) {
        c_arr[i] = (double*)calloc(m->rows, sizeof(double));

        if (c_arr[i] == NULL) {
            for (j = 0; j < i; j++)
                free(c_arr[j]);
            free(c_arr);
            exit(EXIT_FAILURE);
        }
    }
    switch (m->type) {
    case union_var_i:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                c_arr[i][j] = (double)m->arr[i][j].var_i;
            }
        }
        break;

    case union_var_d:
        for (size_t i = 0, j; i < m->rows; ++i) {
            for (j = 0; j < m->cols; ++j) {
                c_arr[i][j] = m->arr[i][j].var_d;
            }
        }
        break;
    }

    det = GaussDet(c_arr, m->cols);
    for (size_t j = 0; j < m->rows; j++)
        free(c_arr[j]);
    free(c_arr);

    return det;
}

// Получение минора матрицы
double**
getminor(TAG_VAR** arr, TYPE_VAR type, int rows, int cols, int row, int col)
{
    int di, dj;

    double** p = (double**)calloc(1, (rows - 1) * sizeof(double*));
    di = 0;
    switch (type) {
    case union_var_i:
        for (int i = 0; i < rows - 1; i++) {
            if (i == row)
                di = 1;
            dj = 0;
            p[i] = (double*)calloc(1, (cols - 1) * sizeof(double));
            for (int j = 0; j < cols - 1; j++) {
                if (j == col)
                    dj = 1;
                p[i][j] = (double)arr[i + di][j + dj].var_i;
            }
        }
        break;

    case union_var_d:
        for (int i = 0; i < rows - 1; i++) {
            if (i == row)
                di = 1;
            dj = 0;
            p[i] = (double*)calloc(1, (cols - 1) * sizeof(double));
            for (int j = 0; j < cols - 1; j++) {
                if (j == col)
                    dj = 1;
                p[i][j] = arr[i + di][j + dj].var_d;
            }
        }
        break;
    }

    return p;
}

// Получение структуры с обратной матрицой (если она существует)
Matrix2d* calc_inverse_matrix(Matrix2d* m)
{
    double** minor;

    if (m == NULL) {
        puts("Указатель пустой.");
        exit(EXIT_FAILURE);
    }
    double det = find_determ(m);

    if (det == 0.f) {
        puts("Обратной матрицы не существует.");
        exit(EXIT_FAILURE);
    }

    Matrix2d* A = calloc(1, sizeof(Matrix2d));
    mem_check(A);
    matrix2d_copy_constructor(m, A);

    for (size_t i = 0, j; i < m->cols; i++) {
        for (j = 0; j < m->cols; j++) {
            minor = getminor(m->arr, m->type, m->cols, m->cols, i, j);
            A->arr[i][j].var_d = GaussDet(minor, m->cols - 1);
            if ((i + j) % 2 == 1)
                A->arr[i][j].var_d *= -1;
            A->arr[i][j].var_d /= det;
            for (size_t i = 0; i < m->cols - 1; i++)
                free(minor[i]);
            free(minor);
            minor = NULL;
        }
    }

    Matrix2d* B = matrix_transpose(A);

    destroy_matrix2d(A, 1);

    return B;
}

#include "bitrgbled.h"

const char* mode_work_types[3] = {"свечение", "мерцание", "затухание"};

// bitrgbled как переменная
// Конструктор
void bitrgbled_constructor(
        ulg* s,
        uch red,
        uch green,
        uch blue,
        uch brightness,
        ushort colortemp,
        uch controltype,
        uch mode)
{
    *s = 0;
    bitrgbled_setred(s, red);
    bitrgbled_setgreen(s, green);
    bitrgbled_setblue(s, blue);
    bitrgbled_setbrightness(s, brightness);
    bitrgbled_setcolortemp(s, colortemp);
    bitrgbled_setcontroltype(s, controltype);
    bitrgbled_setmode(s, mode);
}

// Установка канала красного цвета
void bitrgbled_setred(ulg* s, uch n)
{
    *s &= ~(ulg)RED_MASK;
    *s |= ((ulg)n << RED) & RED_MASK;
}

// Установка канала синего цвета
void bitrgbled_setgreen(ulg* s, uch n)
{
    *s &= ~(ulg)GREEN_MASK;
    *s |= ((ulg)n << GREEN) & GREEN_MASK;
}

// Установка канала зелёного цвета
void bitrgbled_setblue(ulg* s, uch n)
{
    *s &= ~(ulg)BLUE_MASK;
    *s |= ((ulg)n << BLUE) & BLUE_MASK;
}

// Установка яркости
void bitrgbled_setbrightness(ulg* s, uch n)
{
    if (n > BRIGHTNESS_MAX) {
        exit(EXIT_FAILURE);
    }
    *s &= ~(ulg)BRIGHTNESS_MASK;
    *s |= ((ulg)n << BRIGHTNESS) & BRIGHTNESS_MASK;
}

// Установка температуры цвета
void bitrgbled_setcolortemp(ulg* s, ushort n)
{
    if (n < COLORTEMP_MIN) {
        exit(EXIT_FAILURE);
    }
    if (n > COLORTEMP_MAX) {
        exit(EXIT_FAILURE);
    }
    *s &= ~(ulg)COLORTEMP_MASK;
    *s |= ((ulg)n << COLORTEMP) & COLORTEMP_MASK;
}

// Установка типа управления
void bitrgbled_setcontroltype(ulg* s, uch n)
{
    if (n > CONTROL_TYPE_MAX) {
        exit(EXIT_FAILURE);
    }
    *s &= ~(ulg)CONTROL_TYPE_MASK;
    *s |= ((ulg)n << CONTROL_TYPE) & CONTROL_TYPE_MASK;
}

// Установка режима работы
void bitrgbled_setmode(ulg* s, uch n)
{
    if (n > MODE_MAX) {
        exit(EXIT_FAILURE);
    }
    *s &= ~(ulg)MODE_MASK;
    *s |= ((ulg)n << MODE) & MODE_MASK;
}

// Вывод параметров
void show_bitrgbled(ulg* s)
{
    printf("Канал красного цвета: %lu\n", (*s & RED_MASK) >> RED);
    printf("Канал зелёного цвета: %lu\n", (*s & GREEN_MASK) >> GREEN);
    printf("Канал синего цвета: %lu\n", (*s & BLUE_MASK) >> BLUE);
    printf("Уровень яркости: %lu%%\n", (*s & BRIGHTNESS_MASK) >> BRIGHTNESS);
    printf("Температура цвета: %luK\n", (*s & COLORTEMP_MASK) >> COLORTEMP);
    printf("Тип модуля управления: %lu\n",
           (*s & CONTROL_TYPE_MASK) >> CONTROL_TYPE);
    printf("Режим работы: %s\n", mode_work_types[(*s & MODE_MASK) >> MODE]);
}

void show_bitrgbled_bits(ulg s)
{
    static const int size = CHAR_BIT * sizeof(ulg);

    for (int i = size - 1; i >= 0; i--)
        putchar((s >> i) & 1 ? '1' : '0');
    putchar('\n');
}

// bitrgbled как структура
static void mem_check(void* p)
{
    if (p == NULL) {
        free(p);
        puts("Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }
}

// Конструктор
void bitrgbled_struct_constructor(
        BITrgbled* b,
        uch red,
        uch green,
        uch blue,
        uch brightness,
        ushort colortemp,
        uch controltype,
        uch mode)
{
    mem_check(b);
    bitrgbled_struct_setred(b, red);
    bitrgbled_struct_setgreen(b, green);
    bitrgbled_struct_setblue(b, blue);
    bitrgbled_struct_setbrightness(b, brightness);
    bitrgbled_struct_setcolortemp(b, colortemp);
    bitrgbled_struct_setcontroltype(b, controltype);
    bitrgbled_struct_setmode(b, mode);
}

// Установка канала красного цвета
void bitrgbled_struct_setred(BITrgbled* b, uch n)
{
    mem_check(b);
    b->red = n;
}

// Установка канала синего цвета
void bitrgbled_struct_setgreen(BITrgbled* b, uch n)
{
    mem_check(b);
    b->green = n;
}

// Установка канала зелёного цвета
void bitrgbled_struct_setblue(BITrgbled* b, uch n)
{
    mem_check(b);
    b->blue = n;
}

// Установка яркости
void bitrgbled_struct_setbrightness(BITrgbled* b, uch n)
{
    mem_check(b);
    if (n > BRIGHTNESS_MAX) {
        exit(EXIT_FAILURE);
    }
    b->brightness = n;
}

// Установка температуры цвета
void bitrgbled_struct_setcolortemp(BITrgbled* b, ushort n)
{
    mem_check(b);
    if (n < COLORTEMP_MIN) {
        exit(EXIT_FAILURE);
    }
    if (n > COLORTEMP_MAX) {
        exit(EXIT_FAILURE);
    }
    b->colortemp = n;
}

// Установка типа управления
void bitrgbled_struct_setcontroltype(BITrgbled* b, uch n)
{
    mem_check(b);
    if (n > CONTROL_TYPE_MAX) {
        exit(EXIT_FAILURE);
    }
    b->controltype = n;
}

// Установка режима работы
void bitrgbled_struct_setmode(BITrgbled* b, uch n)
{
    mem_check(b);
    if (n > MODE_MAX) {
        exit(EXIT_FAILURE);
    }
    b->mode = n;
}

// Вывод параметров
void show_struct_bitrgbled(BITrgbled* b)
{
    mem_check(b);
    printf("Канал красного цвета: %hu\n", b->red);
    printf("Канал зелёного цвета: %hu\n", b->green);
    printf("Канал синего цвета: %hu\n", b->blue);
    printf("Уровень яркости: %hu%%\n", b->brightness);
    printf("Температура цвета: %uK\n", b->colortemp);
    printf("Тип модуля управления: %hu\n", b->controltype);
    printf("Режим работы: %s\n", mode_work_types[b->mode]);
}

static void printbits(unsigned int num, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
        putchar((num & (1 << i)) ? '1' : '0');
}

void show_struct_bitrgbled_bits(BITrgbled* b)
{
    int size = CHAR_BIT * sizeof(BITrgbled) - STRUCT_BITS;

    for (int i = size - 1; i >= 0; i--)
        putchar('0');

    printbits(b->mode, MODE_BITS);
    printbits(b->controltype, CONTROL_TYPE_BITS);
    printbits(b->colortemp, COLORTEMP_BITS);
    printbits(b->brightness, BRIGHTNESS_BITS);
    printbits(b->blue, COLOR_BITS);
    printbits(b->green, COLOR_BITS);
    printbits(b->red, COLOR_BITS);

    putchar('\n');
}

// Очистка структуры bitrgbled
void destroy_struct_bitrgbled(BITrgbled* b)
{
    free(b);
    b = NULL;
}

// Получение структуры используя переменную
void bitrgbled_struct_from_bitrgbled(ulg* s, BITrgbled* b)
{
    b->red = (*s & RED_MASK) >> RED;
    b->green = (*s & GREEN_MASK) >> GREEN;
    b->blue = (*s & BLUE_MASK) >> BLUE;
    b->brightness = (*s & BRIGHTNESS_MASK) >> BRIGHTNESS;
    b->colortemp = (*s & COLORTEMP_MASK) >> COLORTEMP;
    b->controltype = (*s & CONTROL_TYPE_MASK) >> CONTROL_TYPE;
    b->mode = (*s & MODE_MASK) >> MODE;
}

// Генерация рандомной структуры bitrgbled(переменная)
ulg rand_gen_bitrgbled(void)
{
    ulg settings = 0;

    uch red = (uch)rand();
    uch green = (uch)rand();
    uch blue = (uch)rand();
    uch brightness = (uch)rand() % BRIGHTNESS_MAX;
    ushort colortemp = COLORTEMP_MIN
            + (ushort)rand() % (COLORTEMP_MAX - COLORTEMP_MIN + 1);
    uch controltype = (uch)rand() % CONTROL_TYPE_MAX;
    uch mode = (uch)rand() % MODE_MAX;
    bitrgbled_constructor(
            &settings,
            red,
            green,
            blue,
            brightness,
            colortemp,
            controltype,
            mode);

    return settings;
}

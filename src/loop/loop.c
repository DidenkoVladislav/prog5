#include <lib/bitrgbled.h>

int main(void)
{
    size_t s = 51254564;

    static const int size = CHAR_BIT * sizeof(ulg);

    for (int i = size - 1; i >= 0; i--)
        putchar((s >> i) & 1 ? '1' : '0');
    putchar('\n');

    return 0;
}

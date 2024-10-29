/*
 * Основная цель кода — продемонстрировать использование и размещение данных в разных сегментах памяти
 * (инициализированные и неинициализированные данные, стек, куча).
*/

#include <stdio.h>
#include <stdlib.h>

char globBuf[65536]; /* (bss) Сегмент неинициализированных данных */
int primes[] = { 2, 3, 5, 7 }; /* Сегмент инициализированных данных */

static int square(int x) /* Функция square() имеет свою область в стеке */
{
    int result; /* Локальная переменная в области стека для square() */

    result = x * x;
    return result; /* Значение возвращается через регистр */
}

static void doCalc(int val) /* Функция doCalc() имеет свою область в стеке */
{
    printf("The square of %d is %d\n", val, square(val));

    if (val < 1000) {
        int t; /* Локальная переменная в области стека для doCalc() */

        t = val * val * val;
        printf("The cube of %d is %d\n", val, t);
    }
}

int main(int argc, char *argv[]) /* Функция main() имеет свою область в стеке */
{
    static int key = 9973; /* Сегмент инициализированных данных */
    static char mbuf[10240000]; /* Сегмент неинициализированных данных */
    char *p; /* Локальная переменная в области стека для main() */

    p = malloc(1024); /* Указывает на память в сегменте кучи */

    doCalc(key);

    exit(EXIT_SUCCESS);
}


/*
 модифицируем программу , чтобы она выводила текущее значение программного сегмента (program break) после каждого вызова malloc().
 Это позволит увидеть, что malloc() не вызывает sbrk() для каждого выделения,
 а периодически выделяет более крупные блоки, откуда затем возвращает маленькие куски.

 */

#include "../lib/tlpi_hdr.h"

#define MAX_ALLOCS 1000000

int main(int ac, char *av[])
{
    char *ptr[MAX_ALLOCS];

    int blockSize, numAllocs, j;

    printf("\n");

    if (ac < 3 || strcmp(av[1], "--help") == 0) {
        usageErr("%s num-allocs block-size\n", av[0]);
    }

    numAllocs = getInt(av[1], GN_GT_0, "num-allocs");

    if (numAllocs > MAX_ALLOCS) {
        cmdLineErr("num-allocs > %d", MAX_ALLOCS);
    }

    blockSize = getInt(av[2], GN_GT_0, "block-size");

    printf("Initial program break: %20p\n", sbrk(0));

    // Цикл для выделения памяти и вывода значения программного сегмента после каждого malloc()
    for (j = 0; j < numAllocs; j += 1) {
        ptr[j] = (char *)malloc(blockSize);
        if (ptr[j] == NULL) {
            errExit("malloc");
        }

        // Вывод текущего значения программного сегмента после каждого malloc()
        printf("Program break after malloc() #%d: %20p\n", j+1, sbrk(0));
    }

    printf("Final program break after all malloc() calls: %20p\n", sbrk(0));

    // Освобождение выделенной памяти
    for (j = 0; j < numAllocs; j += 1) {
        free(ptr[j]);
    }

    printf("After free(), program break is %20p\n", sbrk(0));

    exit(EXIT_SUCCESS);
}

/*
Начальная точка: Сначала выводим значение program break, чтобы зафиксировать начальную позицию перед любым выделением памяти.
malloc(): В цикле выделяем память с помощью malloc() в размере blockSize и после каждой операции malloc() выводим значение program break.
Результаты: Запуск программы с небольшим размером blockSize покажет, что program break изменяется не после каждого malloc(), а через несколько вызовов. Это демонстрирует стратегию malloc() — вместо частых небольших запросов sbrk(), он выделяет большие блоки памяти и раздаёт их частями.
free(): После освобождения всех блоков выводим значение program break ещё раз, чтобы увидеть, что освобождённые блоки могут не сразу вернуть program break к изначальному значению.
 */
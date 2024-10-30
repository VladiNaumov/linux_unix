/*
 *
 Программа демонстрирует управление памятью в C, используя malloc и free.
 Она выделяет память в блоках фиксированного размера, а затем освобождает их по заданному шагу,
 отслеживая изменение указателя программного сегмента (program break).

 */

#include "../lib/tlpi_hdr.h"

#define MAX_ALLOCS 1000000  // Максимальное количество блоков для выделения памяти

int main(int ac, char *av[])
{
    char *ptr[MAX_ALLOCS]; // Массив указателей на выделенные блоки памяти

    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

    printf("\n");

    // Проверка на количество аргументов, вывод помощи при необходимости
    if (ac < 3 || strcmp(av[1], "--help") == 0) {
        usageErr("%s num-allocs block-size [step [min [max]]]\n", av[0]);
    }

    // Чтение и проверка аргументов командной строки
    numAllocs = getInt(av[1], GN_GT_0, "num-allocs");
    if (numAllocs > MAX_ALLOCS) {
        cmdLineErr("num-allocs > %d", MAX_ALLOCS);
    }

    blockSize = getInt(av[2], GN_GT_0, "block-size");

    // Опциональные аргументы для шага освобождения и диапазона освобождаемых блоков
    freeStep = (ac > 3) ? getInt(av[3], GN_GT_0, "free-step") : 1;
    freeMin = (ac > 4) ? getInt(av[4], GN_GT_0, "free-min") : 1;
    freeMax = (ac > 5) ? getInt(av[5], GN_GT_0, "free-max") : numAllocs;

    if (freeMax > numAllocs) {
        cmdLineErr("free-max > num-allocs");
    }

    // Вывод текущего значения программного сегмента
    printf("Initial program break: %20p\n", sbrk(0));

    // Выделение памяти в блоках размера blockSize
    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; j += 1) {
        ptr[j] = (char *)malloc(blockSize);
        if (ptr[j] == NULL) {
            errExit("malloc");  // Завершение программы при ошибке выделения
        }
    }

    // Вывод нового значения программного сегмента после malloc
    printf("Program break is now: %20p\n", sbrk(0));

    // Освобождение блоков в диапазоне от freeMin до freeMax с шагом freeStep
    printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j += freeStep) {
        free(ptr[j]);
    }

    // Вывод значения программного сегмента после освобождения памяти
    printf("After free(), program break is %20p\n", sbrk(0));

    exit(EXIT_SUCCESS);
}

/*

Аргументы командной строки задают параметры выделения и освобождения памяти:количество блоков, размер блока, шаг и диапазон освобождения.
malloc выделяет память в блоках фиксированного размера. Мы отслеживаем program break (границу динамически выделенной памяти) до и после выделения.
free освобождает блоки по шагу, указанному в командной строке, снова отслеживая program break после освобождения памяти.
Программа демонстрирует, как динамическое выделение/освобождение памяти влияет на program break — индикатор текущего состояния памяти процесса.

 */
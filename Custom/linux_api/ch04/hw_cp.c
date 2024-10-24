/*
 * Программа для копирования файлов с поддержкой дыр (последовательностей нулевых байтов), аналогичная cp,
 * может быть реализована с использованием системных вызовов read(), write() и lseek().
 * Дыры в файле могут быть созданы с помощью lseek() для перемещения указателя файла, минуя последовательности нулевых байтов,
 * вместо их записи.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "../lib/tlpi_hdr.h"

#ifndef BUF_SIZE
#define BUF_SIZE 1024  // Размер буфера для чтения данных
#endif

int main(int ac, char *av[])
{
    int inputFd, outputFd;      // Дескрипторы для исходного и целевого файлов
    ssize_t numRead;            // Количество прочитанных байт
    char buf[BUF_SIZE];         // Буфер для чтения данных
    off_t holeStart, holeEnd;   // Позиции для обработки дыр

    if (ac != 3) {  // Проверяем количество аргументов
        usageErr("%s old-file new-file\n", av[0]);
    }

    // Открываем исходный файл для чтения
    inputFd = open(av[1], O_RDONLY);
    if (inputFd == -1) {
        errExit("opening file %s", av[1]);
    }

    // Открываем или создаем новый файл для записи
    outputFd = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (outputFd == -1) {
        errExit("opening file %s", av[2]);
    }

    // Цикл для чтения данных из исходного файла и их копирования в целевой файл
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
        int i = 0;
        while (i < numRead) {
            // Поиск начала дыры (нулевые байты)
            if (buf[i] == '\0') {
                holeStart = i;
                // Продолжаем искать нулевые байты до конца последовательности
                while (i < numRead && buf[i] == '\0') {
                    i++;
                }
                holeEnd = i;

                // Перемещаем указатель в целевом файле на длину дыры
                if (lseek(outputFd, holeEnd - holeStart, SEEK_CUR) == -1) {
                    errExit("lseek");
                }
            } else {
                // Копируем данные, которые не являются дырой
                int nonHoleStart = i;
                // Продолжаем копировать, пока не встретим дыры или не закончим буфер
                while (i < numRead && buf[i] != '\0') {
                    i++;
                }
                if (write(outputFd, buf + nonHoleStart, i - nonHoleStart) != (i - nonHoleStart)) {
                    errExit("write");
                }
            }
        }
    }

    if (numRead == -1) {
        errExit("read");
    }

    // Закрытие файлов
    if (close(inputFd) == -1) {
        errExit("close input");
    }
    if (close(outputFd) == -1) {
        errExit("close output");
    }

    exit(EXIT_SUCCESS);
}

/**
 * Пояснение:
Открытие файлов: Исходный файл открывается на чтение с помощью open(), а целевой файл создается или перезаписывается с флагами O_WRONLY | O_CREAT | O_TRUNC.

Чтение файла: Программа читает исходный файл блоками фиксированного размера (например, 1024 байта).

Обработка дыр: Если в буфере встречается последовательность нулевых байтов (дыра), программа использует lseek(), чтобы переместить указатель в целевом файле на длину этой последовательности, не записывая нулевые байты.

Запись данных: Если в буфере встречаются ненулевые байты, они записываются в целевой файл с помощью write().

Закрытие файлов: После копирования файловые дескрипторы закрываются с помощью close().

Особенности:
Использование lseek() для создания дыр в целевом файле без явной записи нулевых байтов.
Это позволяет более эффективно копировать файлы с дырками, экономя пространство на диске.

 */
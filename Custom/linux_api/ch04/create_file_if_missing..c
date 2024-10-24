/*
 * Этот код демонстрирует, как один процесс проверяет наличие файла, а затем может создать его, если он не существует.
 * Это может быть полезно в ситуациях, где нужно избежать гонок данных между процессами при создании файла. Рассмотрим ключевые моменты:
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../lib/tlpi_hdr.h"

int main(int ac, char *av[])
{
    int fd = open(av[1], O_WRONLY); // Попытка открыть файл только на запись

    if (fd != -1) {  // Успех открытия файла (файл существует)
        printf("[PID %ld] File \"%s\" already exists\n", (long)getpid(), av[1]);
        close(fd);  // Закрываем файл, так как нам нужно только проверить его наличие
    } else {
        // Если файл не удалось открыть по неожиданной причине
        if (errno != ENOENT) {  // ENOENT означает, что файл не существует
            errExit("open");  // Если ошибка другая, выводим сообщение и завершаем
        } else {
            // Сценарий, когда файл не существует
            printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long)getpid(), av[1]);

            // Если в командной строке передан второй аргумент, симулируем задержку
            if (ac > 2) {
                sleep(5);  // Процесс "засыпает" на 5 секунд
                printf("[PID %ld] Done sleeping\n", (long)getpid());
            }

            // Пытаемся создать файл с правами чтения и записи для владельца
            fd = open(av[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1) {  // Если не удалось создать файл
                errExit("open");
            }

            // Файл успешно создан
            printf("[PID %ld] Created file \"%s\" exclusively\n", (long)getpid(), av[1]);
        }
    }

    return 0;
}
/*

### Основные моменты:

1. **Проверка существования файла**:
- Открытие файла с флагом `O_WRONLY` проверяет, существует ли файл. Если `open()` успешен, файл существует, и программа выводит соответствующее сообщение.
- Если файл не существует, `open()` вернёт `-1`, и `errno` будет установлен в `ENOENT`, что указывает на отсутствие файла.

2. **Обработка ошибок**:
- Если ошибка открытия файла связана не с отсутствием файла (например, недостаток прав доступа или другие ошибки), программа завершает выполнение с сообщением об ошибке.

3. **Задержка выполнения**:
- Если передан второй аргумент, программа "засыпает" на 5 секунд, симулируя возможную задержку между процессами. Это может быть полезно для проверки синхронизации процессов, например, чтобы убедиться, что другой процесс пытается создать тот же файл.

4. **Создание файла**:
- Если файл не существует, программа создаёт его с правами на чтение и запись для владельца (`S_IRUSR | S_IWUSR`).

5. **Конкуренция процессов**:
- Если несколько процессов выполняют эту программу одновременно, могут возникнуть условия гонки при проверке существования файла и его создании.

 */
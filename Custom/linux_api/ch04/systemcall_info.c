/* 4. File I/O: The Universal I/O */


/*
    В ранних реализациях UNIX open() имел только два аргумента и не мог создавать
    новый файл. Для создания и открытия нового файла использовался системный вызов creat().
*/
int creat(const char *pathname, mode_t mode);
// Возвращает файловый дескриптор или -1 при ошибке


/*
    Системный вызов close() закрывает открытый файловый дескриптор, освобождая его
    для повторного использования процессом. Когда процесс завершается, все его открытые
    файловые дескрипторы автоматически закрываются.
*/
int close(int fd);
// Возвращает 0 при успешном выполнении или -1 при ошибке

/*
    Для каждого открытого файла ядро записывает смещение файла, иногда также называемое
    указателем на чтение/запись. Это позиция в файле, с которой начнется следующая
    операция read() или write().
*/
off_t lseek(int fd, off_t offset, int whence);
// Возвращает новое смещение файла при успехе или -1 при ошибке


/*
    Системный вызов ioctl() — это универсальный механизм для выполнения операций с файлами и
    устройствами, которые не укладываются в общую модель ввода-вывода.
*/
int ioctl(int fd, int request, ... /* argp */);
// Возвращаемое значение при успешном выполнении зависит от запроса или -1 при ошибке


/* Системный вызов fcntl() выполняет ряд управляющих операций над открытым файловым дескриптором. */
#include <fcntl.h>
int fcntl(int fd, int cmd, ...);
// Возвращаемое значение при успехе зависит от cmd или -1 при ошибке

/*
    Вызов dup() принимает старый дескриптор файла oldfd и возвращает новый дескриптор,
    который ссылается на то же открытое описание файла. Новый дескриптор гарантированно
    будет наименьшим неиспользуемым дескриптором файла.
*/
#include <unistd.h>
int dup(int oldfd);
int dup2(int oldfd, int newfd);
int dup3(int oldfd, int newfd, int flags);
// Возвращает (новый) файловый дескриптор при успехе или -1 при ошибке


/* Системные вызовы readv() и writev() выполняют scatter-gather ввод-вывод. */
#include <sys/uio.h>
ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
// Возвращает количество прочитанных байтов, 0 при EOF или -1 при ошибке
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
// Возвращает количество записанных байтов или -1 при ошибке


/* Функция mkstemp() генерирует уникальное имя файла на основе шаблона, указанного вызвавшим процессом, и открывает файл, возвращая файловый дескриптор. */
#include <stdlib.h>
int mkstemp(char *template);
// Возвращает файловый дескриптор при успехе или -1 при ошибке

/* Функция tmpfile() создает временный файл с уникальным именем, открытый для чтения и записи. */
#include <stdio.h>
FILE *tmpfile(void);
// Возвращает указатель на файл при успехе или NULL при ошибке

/*Важные дополнения:
- dup и dup2: Дублирование дескрипторов позволяет направлять вывод одного файла в другой, что может быть полезно для перенаправления ввода/вывода.
- pread и pwrite: Независимо от текущего смещения, указание конкретной позиции в файле позволяет работать с данными в произвольной позиции, что может быть полезно для записи логов и работы с базами данных.*/


/*
 * Пример использования open() и close():
 * open() открывает файл (создаёт его, если не существует), а close() закрывает.
 * Мы можем задать режим доступа (например, только чтение или чтение и запись).
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);  // Создаем или открываем файл для записи
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    // Записываем данные в файл
    write(fd, "Hello, World!\n", 14);
    close(fd);  // Закрываем файл

    return 0;
}

/*
 * Пример использования read() и write():
 * read() читает данные из файла, а write() записывает их.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDONLY); // Открываем файл только для чтения
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer));  // Читаем данные в buffer
    if (bytes_read == -1) {
        perror("read failed");
        close(fd);
        return 1;
    }

    // Печатаем прочитанные данные
    write(STDOUT_FILENO, buffer, bytes_read);
    close(fd);

    return 0;
}

/*
 * Пример использования dup() и dup2():
 * dup() дублирует файловый дескриптор, позволяя перенаправить вывод,
 * например, в файл, а не в стандартный вывод.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    int new_fd = dup2(fd, STDOUT_FILENO);  // Перенаправляем stdout в файл
    if (new_fd == -1) {
        perror("dup2 failed");
        close(fd);
        return 1;
    }

    printf("This message will be written to the file instead of the console.\n");
    close(fd);

    return 0;
}

/*
 * Пример использования lseek():
 * Позволяет устанавливать положение курсора чтения/записи внутри файла.
 * Например, можно переместить курсор в начало, в конец или в определённую позицию.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    lseek(fd, 0, SEEK_END);  // Перемещаем указатель на конец файла
    write(fd, "\nAppended Text", 14);  // Дописываем данные в конец

    close(fd);
    return 0;
}

/*
 * Пример использования pread() и pwrite():
 * Чтение и запись по определённому смещению без изменения текущего положения указателя.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_RDWR);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    char buffer[100];
    pread(fd, buffer, sizeof(buffer), 0);  // Чтение с начала файла
    printf("Read from file: %s\n", buffer);

    pwrite(fd, "Random Access", 13, 50);  // Запись по смещению 50 байт

    close(fd);
    return 0;
}

/*
 * Пример использования truncate():
 * Устанавливаем новый размер файла (сокращаем или удлиняем).
 */

#include <unistd.h>
#include <stdio.h>

int main() {
    if (truncate("example.txt", 50) == -1) { // Сокращаем файл до 50 байт
        perror("truncate failed");
        return 1;
    }

    printf("File truncated successfully.\n");
    return 0;
}

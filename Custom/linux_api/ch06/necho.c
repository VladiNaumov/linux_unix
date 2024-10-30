/*
 * Этот код выводит каждый аргумент командной строки, переданный программе, на экран.
 * Он демонстрирует, как получить доступ к аргументам, переданным через argv, и распечатать их вместе с индексом.
 * */

#include "../lib/tlpi_hdr.h" /* Подключение пользовательской библиотеки с определениями и макросами (например, для EXIT_SUCCESS) */

int main(int ac, char *av[]) /* Главная функция, получает количество аргументов и массив строк с аргументами */
{
    int j;

    for (j = 0; j < ac; j += 1) { /* Цикл по всем аргументам */
        printf("av[%d] = %s\n", j, av[j]); /* Выводит индекс и значение каждого аргумента */
    }
    exit(EXIT_SUCCESS); /* Завершение программы с кодом успеха */
}
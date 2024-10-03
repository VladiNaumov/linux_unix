/* кода, который иллюстрирует работу с макросами alloc_thread_info, free_thread_info, и task_thread_info */

#include <linux/kernel.h>      // Для printk
#include <linux/sched.h>       // Для структуры task_struct
#include <linux/slab.h>        // Для kmalloc и kfree
#include <linux/gfp.h>         // Для флагов GFP_KERNEL
#include <linux/init.h>        // Для макросов __init и __exit
#include <linux/module.h>      // Для модулей ядра

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example of thread_info macros usage");

// Функция для создания и освобождения thread_info
static int __init example_init(void) {
    struct thread_info *tinfo;
    struct task_struct *task = current;  // Получаем текущий процесс

    printk(KERN_INFO "Current process: %s (pid: %d)\n", task->comm, task->pid);

    // Выделение thread_info для текущего потока
    tinfo = alloc_thread_info(task);
    if (!tinfo) {
        printk(KERN_ERR "Failed to allocate thread_info\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Allocated thread_info at: %p\n", tinfo);

    // Получение thread_info через макрос task_thread_info
    struct thread_info *current_tinfo = task_thread_info(task);
    printk(KERN_INFO "Current thread_info via task_thread_info: %p\n", current_tinfo);

    // Освобождение thread_info
    free_thread_info(tinfo);
    printk(KERN_INFO "Freed thread_info\n");

    return 0;
}

// Функция для выгрузки модуля
static void __exit example_exit(void) {
    printk(KERN_INFO "Module unloaded\n");
}

module_init(example_init);
module_exit(example_exit);

/*
Пояснения:
current — это глобальная переменная, указывающая на текущую структуру задачи (task_struct) для выполняемого процесса. Она используется для получения текущего потока.

alloc_thread_info(task) — этот макрос выделяет память для структуры thread_info, ассоциированной с текущим процессом.

task_thread_info(task) — макрос возвращает указатель на структуру thread_info для текущего процесса. Это полезно, если вы хотите получить информацию о текущем потоке.

free_thread_info(tinfo) — освобождает ранее выделенную структуру thread_info.

printk — используется для вывода сообщений в логи ядра. Мы выводим информацию о процессе, памяти и структуре thread_info.

Как это работает:
При загрузке модуля (example_init), он выделяет память для структуры thread_info и выводит её адрес в логи ядра.
После этого, с помощью task_thread_info, выводится информация о thread_info, связанной с текущим потоком.
При завершении работы, выделенная память освобождается с помощью free_thread_info.
 */
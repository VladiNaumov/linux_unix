 *	header.S
 *
 *	Авторские права (C) 1991, 1992 Линус Торвальдс
 *
 *	Основан на bootsect.S и setup.S
 *	модифицировано множеством людей
 *
 *	Переписано как общий файл Х. Питером Анвином (апрель 2007)
 *
 * БОЛЬШАЯ ЖИРНАЯ ЗАМЕТКА: Мы находимся в реальном режиме, используя сегменты по 64Кб.
 * Поэтому сегментные адреса необходимо умножать на 16, чтобы получить их соответствующие
 * линейные адреса. Чтобы избежать путаницы, линейные адреса записаны с лидирующим
 * префиксом в шестнадцатеричном формате, а сегментные адреса написаны в виде сегмент:смещение.
 *
 */

BOOTSEG		= 0x07C0		/* исходный адрес загрузочного сектора */
SYSSEG		= 0x1000		/* исторический адрес загрузки >> 4 */

#ifndef SVGA_MODE
#define SVGA_MODE ASK_VGA
#endif

#ifndef RAMDISK
#define RAMDISK 0
#endif

#ifndef ROOT_RDONLY
#define ROOT_RDONLY 1
#endif

	.code16
	.section ".bstext", "ax"

	.global bootsect_start

bootsect_start:
	# Нормализация стартового адреса
	ljmp	$BOOTSEG, $start2

start2:
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %ss
	xorw	%sp, %sp
	sti
	cld

	movw	$bugger_off_msg, %si

msg_loop:
	lodsb
	andb	%al, %al
	jz	bs_die
	movb	$0xe, %ah
	movw	$7, %bx
	int	$0x10
	jmp	msg_loop

bs_die:
	# Позволить пользователю нажать клавишу, затем перезагрузиться
	xorw	%ax, %ax
	int	$0x16
	int	$0x19

	# int 0x19 не должен возвращать управление. На случай, если это всё-таки произойдёт,
	# вызвать код перезагрузки BIOS...
	ljmp	$0xf000,$0xfff0

	.section ".bsdata", "a"

bugger_off_msg:
	.ascii	"Прямая загрузка с дискеты больше не поддерживается.\r\n"
	.ascii	"Пожалуйста, используйте программу-загрузчик.\r\n"
	.ascii	"\n"
	.ascii	"Удалите диск и нажмите любую клавишу для перезагрузки . . .\r\n"
	.byte	0


	# Атрибуты ядра; используются setup. Это часть 1 заголовка, из старого загрузочного сектора.

	.section ".header", "a"
	.globl	hdr
hdr:
setup_sects:	.byte 0			/* Заполняется build.c */
root_flags:	.word ROOT_RDONLY
syssize:	.long 0			/* Заполняется build.c */
ram_size:	.word 0			/* Устарело */
vid_mode:	.word SVGA_MODE
root_dev:	.word 0			/* Заполняется build.c */
boot_flag:	.word 0xAA55

	# смещение 512, точка входа

	.globl	_start
_start:
		# Явно указываем это как байты, иначе ассемблер
		# попытается сгенерировать 3-байтовый прыжок, что
		# приведет к смещению всего остального.
		.byte	0xeb		# короткий (2-байтовый) прыжок
		.byte	start_of_setup-1f
1:

	# Часть 2 заголовка, из старого setup.S

		.ascii	"HdrS"		# сигнатура заголовка
		.word	0x020a		# номер версии заголовка (>= 0x0105)
					# иначе старая версия loadlin-1.5 выйдет из строя
		.globl realmode_swtch
realmode_swtch:	.word	0, 0		# default_switch, SETUPSEG
start_sys_seg:	.word	SYSSEG		# устарело и не имеет смысла, но
					# на случай, если что-то решит это "использовать"
		.word	kernel_version-512 # указывает на строку версии ядра
					# вышеупомянутая часть заголовка совместима
					# с loadlin-1.5 (версия заголовка 1.5). Не
					# изменяйте это.



### 2. 


type_of_loader:	.byte	0		# 0 означает древний загрузчик, новые
					# загрузчики знают, что это нужно изменить.
					# Смотрите Documentation/i386/boot.txt для
					# назначенных идентификаторов.

# Флаги, неиспользуемые биты должны быть нулями (RFU) бит в loadflags
loadflags:
LOADED_HIGH	= 1			# Если установлен, ядро загружено высоко
CAN_USE_HEAP	= 0x80			# Если установлен, загрузчик также установил
					# heap_end_ptr, чтобы указать, сколько
					# пространства за setup.S можно использовать
					# для нужд кучи.
					# Только загрузчик знает, что свободно.
		.byte	LOADED_HIGH

setup_move_size: .word  0x8000		# размер перемещения, когда setup не
					# загружен по адресу 0x90000. Мы переместим setup
					# в 0x90000 перед прыжком в ядро. Однако только
					# загрузчик знает, сколько данных за нами
					# также нужно загрузить.

code32_start:				# здесь загрузчики могут указать другой
					# адрес для начала 32-битного кода.
		.long	0x100000	# 0x100000 = значение по умолчанию для большого ядра.

ramdisk_image:	.long	0		# адрес загруженного образа ramdisk
					# Здесь загрузчик указывает 32-битный
					# адрес, куда он загрузил образ.
					# Это будет прочитано только ядром.

ramdisk_size:	.long	0		# его размер в байтах.

bootsect_kludge:
		.long	0		# устаревшее.

heap_end_ptr:	.word	_end+STACK_SIZE-512
					# (Версия заголовка 0x0201 или позже)
					# пространство отсюда (исключительно) до
					# конца кода setup можно использовать
					# для локальных нужд кучи.

ext_loader_ver:
		.byte	0		# Расширенная версия загрузчика.
ext_loader_type:
		.byte	0		# Расширенный тип загрузчика.

cmd_line_ptr:	.long	0		# (Версия заголовка 0x0202 или позже)
					# Если не равно нулю, это 32-битный указатель
					# на командную строку ядра.
					# Командная строка должна находиться
					# между началом setup и концом
					# нижней памяти (0xa0000), иначе она
					# может быть перезаписана до того,
					# как будет прочитана. Если это поле
					# используется, больше нет ничего
					# магического в сегменте 0x90000; setup
					# может быть расположен где угодно в
					# низкой памяти от 0x10000 или выше.

ramdisk_max:	.long 0x7fffffff
					# (Версия заголовка 0x0203 или позже)
					# Наивысший безопасный адрес для
					# содержимого initrd.
					# Текущее ядро позволяет до 4 ГБ,
					# но оставлено 2 ГБ для избежания
					# возможных ошибок загрузчика.

kernel_alignment:  .long CONFIG_PHYSICAL_ALIGN	# физическое выравнивание адреса,
						# необходимое для защищённого режима
						# ядра.

#ifdef CONFIG_RELOCATABLE
relocatable_kernel:    .byte 1
#else
relocatable_kernel:    .byte 0
#endif
min_alignment:		.byte MIN_KERNEL_ALIGN_LG2	# минимальное выравнивание.
pad3:			.word 0

cmdline_size:   .long   COMMAND_LINE_SIZE-1     # длина командной строки,
                                                # добавлено с версией протокола загрузки 2.06.

hardware_subarch:	.long 0			# подархитектура, добавлено с версией 2.07,
						# по умолчанию 0 для обычного x86 ПК.

hardware_subarch_data:	.quad 0

payload_offset:		.long ZO_input_data
payload_length:		.long ZO_z_input_len

setup_data:		.quad 0			# 64-битный физический указатель на
						# односвязный список
						# структуры setup_data.

pref_address:		.quad LOAD_PHYSICAL_ADDR	# предпочтительный адрес загрузки.

#define ZO_INIT_SIZE	(ZO__end - ZO_startup_32 + ZO_z_extract_offset)
#define VO_INIT_SIZE	(VO__end - VO__text)
#if ZO_INIT_SIZE > VO_INIT_SIZE
#define INIT_SIZE ZO_INIT_SIZE
#else
#define INIT_SIZE VO_INIT_SIZE
#endif
init_size:		.long INIT_SIZE		# размер инициализации ядра.

# Конец заголовка настройки #####################################################


# 3
	.section ".entrytext", "ax"
start_of_setup:
#ifdef SAFE_RESET_DISK_CONTROLLER
# Сброс контроллера дисков.
	movw	$0x0000, %ax		# Сбросить контроллер дисков
	movb	$0x80, %dl		# Все диски
	int	$0x13
#endif

# Принудительно установить %es = %ds
	movw	%ds, %ax
	movw	%ax, %es
	cld

# Очевидно, что некоторые древние версии LILO вызывали ядро с %ss != %ds,
# что случайно работало в старом коде. Пересчитаем указатель стека, если %ss неверен.
# В противном случае оставим его как есть, так как LOADLIN настраивает стек за своим кодом,
# поэтому мы не можем слепо установить его сразу после кучи.

	movw	%ss, %dx
	cmpw	%ax, %dx	# %ds == %ss?
	movw	%sp, %dx
	je	2f		# -> предположить, что %sp настроен корректно

	# Неверный %ss, создаем новый стек
	movw	$_end, %dx
	testb	$CAN_USE_HEAP, loadflags
	jz	1f
	movw	heap_end_ptr, %dx
1:	addw	$STACK_SIZE, %dx
	jnc	2f
	xorw	%dx, %dx	# Предотвратить переполнение

2:	# Теперь %dx должен указывать на конец пространства для стека
	andw	$~3, %dx	# выравнивание по границе dword (раз уж на то пошло...)
	jnz	3f
	movw	$0xfffc, %dx	# Убедиться, что не ноль
3:	movw	%ax, %ss
	movzwl	%dx, %esp	# Очистить верхнюю половину %esp
	sti			# Теперь у нас должен быть рабочий стек

# Мы вошли с %cs = %ds+0x20, нормализуем %cs, чтобы
# он соответствовал другим сегментам.
	pushw	%ds
	pushw	$6f
	lretw
6:

# Проверка сигнатуры в конце setup
	cmpl	$0x5a5aaa55, setup_sig
	jne	setup_bad

# Обнуление секции .bss
	movw	$__bss_start, %di
	movw	$_end+3, %cx
	xorl	%eax, %eax
	subw	%di, %cx
	shrw	$2, %cx
	rep; stosl

# ### Переход к C-коду (не должно вернуться) управление передается функции asmlinkage void __init start_kernel(void) ###
	calll	main

# Ошибка в setup...
setup_bad:
	movl	$setup_corrupt, %eax
	calll	puts
	# Переход к следующему блоку...

	.globl	die
	.type	die, @function
die:
	hlt
	jmp	die

	.size	die, .-die

	.section ".initdata", "a"
setup_corrupt:
	.byte	7
	.string	"No setup signature found...\n"

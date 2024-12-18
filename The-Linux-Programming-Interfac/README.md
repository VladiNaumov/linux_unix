Gidday!

This is the code for the book "The Linux Programming Interface",
along with other code that I use in various conference presentations,
and this is a note from me, the author, Michael Kerrisk.

For instructions on building the programs, see the file BUILDING.

For notes on changes that have been made to the code since it was
published in the book, see the file CHANGES.

If you've come here looking for code examples from a particular
conference presentation that I've made, then after unpacking the
source code, you should find the desired example(s) at the pathname(s)
shown in the conference presentation.



Subdirectories
==============

Under the 'tlpi' directory are a number of subdirectories. Each
subdirectory corresponds to one or more chapters of the book.
The following paragraphs give brief notes on the contents of
each subdirectory.

Note that in some cases, files are (hard) linked to appear in more than
one directory. This is particularly the case for each of the files in
the 'lib' directory, most of which are also linked in the directory
of the chapter relating to that file.

Directory       Files for Chapter...

lib             This contains library routines used by other
                programs. The tlpi_hdr.h and error_functions.*
                files are located here.

progconc        3 (System Programming Concepts)

fileio          4 and 5 (File I/O)

proc            6 (Processes)

memalloc        7 (Memory Allocation)

users_groups    8 (Users and Groups)

proccred        9 (Process Credentials)

time            10 (Time)

syslim          11 (System Limits and Options)

sysinfo         12 (System and Process Information)

filebuff        13 (File I/O Buffering)

filesys         14 (File Systems)

files           15 (File Attributes)

xattr           16 (Extended Attributes)

acl             17 (Access Control Lists)

dirs_links      18 (Directories and Links)

inotify         19 (Monitoring File Events)

signals         20 to 22 (Signals)

timers          23 (Timers and Sleeping)

procexec        24 (Process Creation), 25 (Process Termination),
                26 (Monitoring Child Processes), 27 (Program Execution),
                and 28 (Further Details on Process Creation and Program
                Execution)

threads         29 to 33 (POSIX Threads)

pgsjc           34 (Process Groups, Sessions, and Job Control)

procpri         35 (Process Priorities and Scheduling)

procres         36 (Process Resources)

daemons         37 (Daemons)

cap             39 (Capabilities)

loginacct       40 (Login Accounting)

shlibs          41 and 42 (Shared Libraries)

pipes           44 (Pipes and FIFOs)

svipc           45 (System V IPC)

svmsg           46 (System V Message Queues)

svsem           47 (System V Semaphores)

svshm           48 (System V Shared Memory)

mmap            49 (Memory Mappings)

vmem            50 (Virtual Memory Operations)

pmsg            52 (POSIX Message Queues)

psem            53 (POSIX Semaphores)

pshm            54 (POSIX Shared Memory)

filelock        55 (File Locking)

sockets         56 to 61 (Sockets and Network Programming)

tty             62 (Terminals)

altio           63 (Alternative I/O Models)

pty             64 (Pseudoterminals)

getopt          Appendix B: Parsing Command-Line Options

In addition, the following supplementary code is included (relating
to topics NOT covered in TLPI):

namespaces      Code examples for namespaces, mainly related to my LWN.net
                article series starting at https://lwn.net/Articles/531114/

seccomp         Code examples demonstrating the use of the seccomp
                (Secure Computing) facility.

vdso            Some example code relating to the VDSO (Virtual Dynamic
                Shared Object); see vdso(7)
Source code licensing
=====================

All complete programs provided in this distribution are covered by
the GNU General Public License (Version 3), a copy of which is
contained in the file COPYING.gpl-v3, which should have arrived with
this tarball.  The library functions (in the lib/ directory) are
covered by the GNU Lesser General Public License (Version 3); see the
file COPYING.lgpl-v3 provided with this tarball.


A note on the source code
=========================

The source code is available in two versions: "dist" and "book".
The "book" version contains the program source files as published in
the book. The source files in the "dist" version contain extra code
beyond that published in the book. The differences between the "dist"
and "book" versions are as follows:

a) The "dist" versions of some programs contain extra comments.
   These additional comments were stripped out of the printed version
   to make the published versions of the programs shorter. (The book
   itself contains text describing the operation of the programs.)

b) In a few cases, some changes have been incorporated into the
   "dist" versions to make it possible to compile programs on UNIX
   implementations other than Linux, so that you can try them out
   on other implementations if you wish.  Where necessary, the
   additional code is conditionally compiled using the following
   platform-specific macros:

        __linux__       Defined on Linux
        __sun           Defined on Solaris
        __FreeBSD__     Defined on FreeBSD
        __NetBSD__      Defined on NetBSD
        __OpenBSD__     Defined on OpenBSD
        __APPLE__       Defined on Mac OS X
        __hpux          Defined on HP-UX
        __osf__         Defined on Tru64 UNIX (formerly DEC OSF1)
        __sgi           Defined on Irix
        _AIX            Defined on AIX

c) In the "dist" version, some programs have extra functionality beyond
   that in the "book" versions. Where this is significant, comments in
   the programs explain the differences.

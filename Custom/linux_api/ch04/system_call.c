#include <sys/stat.h>
#include <fcntl.h>

/* The open() system call either opens an existing file or creates and opens a new file. */
int open(const char * pathname , int  flags , ... /* mode_t  mode  */);
//Returns file descriptor on success, or –1 on error

/*
    In early UNIX implementations, open() had only two arguments and could not be
    used to create a new file. Instead, the creat() system call was used to create and open
    a new file.
*/
int creat(const char * pathname , mode_t  mode );
// Returns file descriptor, or –1 on error

/* The read() system call reads data from the open file referred to by the descriptor fd. */
ssize_t read(int  fd , void * buffer , size_t  count );
// Returns number of bytes read, 0 on EOF, or –1 on error

/* The write() system call writes data to an open file. */
ssize_t write(int fd, void * buffer , size_t  count );
// Returns number of bytes written, or –1 on error

/*
    The close() system call closes an open file descriptor, freeing it for subsequent reuse
    by the process. When a process terminates, all of its open file descriptors are automatically closed.
 */
int close(int  fd );
// Returns 0 on success, or –1 on error


/*
    For each open file, the kernel records a file offset, sometimes also called the read-
    write offset or pointer. This is the location in the file at which the next read() or write()
    will commence. The file offset is expressed as an ordinal byte position relative to
    the start of the file. The first byte of the file is at offset 0.
    The file offset is set to point to the start of the file when the file is opened and
    is automatically adjusted by each subsequent call to read() or write() so that it points
    to the next byte of the file after the byte(s) just read or written. Thus, successive
    read() and write() calls progress sequentially through a file.
    The lseek() system call adjusts the file offset of the open file referred to by the
    file descriptor fd, according to the values specified in offset and whence.
 */

off_t lseek(int  fd , off_t  offset , int  whence );
//Returns new file offset if successful, or –1 on error

/*
    The  ioctl()  system  call  is  a  general-purpose  mechanism  for  performing  file  and
    device operations that fall outside the universal I/O model described earlier in this
    chapter.
 */
int ioctl(int  fd , int  request , ... /*  argp  */);
//Value returned on success depends on request, or –1 on error


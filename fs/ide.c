/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
//      read data from IDE disk. First issue a read request through
//      disk register and then copy data from disk buffer
//      (512 bytes, a sector) to destination array.
//
// Parameters:
//      diskno: disk number.
//      secno: start sector number.
//      dst: destination for data read from IDE disk.
//      nsecs: the number of sectors to read.
//
// Post-Condition:
//      If error occurred during read the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
        // 0x200: the size of a sector: 512 bytes.
        int offset_begin = secno * 0x200;
        int offset_end = offset_begin + nsecs * 0x200;
        int offset = 0;
        int tmp;

        while (offset_begin + offset < offset_end) {
                syscall_write_dev(&diskno,0x13000010,4);
                tmp = offset_begin + offset;
                syscall_write_dev(&tmp,0x13000000,4);
                int zero = 0;
                syscall_write_dev(&zero,0x13000020,1);
                syscall_read_dev(&tmp,0x13000030,4);
                if (tmp == 0) {
                        user_panic("read IDE disk error");
                }
                if (syscall_read_dev(dst+offset,0x13004000,0x200)) {
                        user_panic("read dev error");
                }
                offset = offset + 0x200;
        }
}


// Overview:
//      write data to IDE disk.
//
// Parameters:
//      diskno: disk number.
//      secno: start sector number.
//      src: the source data to write into IDE disk.
//      nsecs: the number of sectors to write.
//
// Post-Condition:
//      If error occurred during read the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
        int offset_begin = secno * 0x200;
        int offset_end = offset_begin + nsecs * 0x200;
        int offset = 0;
        writef("diskno: %dn", diskno);
        int tmp;
        while (offset_begin + offset < offset_end) {
                if (syscall_write_dev(src+offset,0x13004000,0x200)) {
                        user_panic("write dev error");
                }
                syscall_write_dev(&diskno,0x13000010,4);
                tmp = offset_begin + offset;
                syscall_write_dev(&tmp,0x13000000,4);
                int one = 1;
                int oneone = one << 24;
                syscall_write_dev(&oneone,0x13000020,1);
                syscall_read_dev(&tmp,0x13000030,4);
                if (tmp == 0) {
                        user_panic("write IDE disk error");
                }
                offset = offset + 0x200;
        }
}



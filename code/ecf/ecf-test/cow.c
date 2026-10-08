/* cow.c - demonstrate fork() copy-on-write by printing PHYSICAL addresses.
 *
 * The physical address is obtained from /proc/self/pagemap: the 64-bit entry
 * for a virtual page holds the page frame number (PFN) in bits 0..54.
 *   phys_addr = (PFN << 12) | (vaddr & 0xfff)          (4 KB pages)
 *
 * NOTE: kernels >= 4.0 hide the PFN from non-root users (bits 0..54 are
 * zeroed unless the reader has CAP_SYS_ADMIN), so run this with sudo.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

static long pagesize;

/* Return the physical address backing virtual address `va`, or 0 if the
 * page is not resident in RAM or the PFN is hidden from us (no privilege). */
static uint64_t phys_addr(volatile void *va)
{
    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) {
        perror("open /proc/self/pagemap");
        return 0;
    }
    uintptr_t vaddr = (uintptr_t)va;
    off_t off = (off_t)(vaddr / pagesize) * sizeof(uint64_t);
    uint64_t entry = 0;
    if (lseek(fd, off, SEEK_SET) < 0)
        perror("lseek");
    if (read(fd, &entry, sizeof entry) != (ssize_t)sizeof entry)
        perror("read");
    close(fd);

    if (!(entry & (1ULL << 63)))           /* bit 63: page present in RAM */
        return 0;
    uint64_t pfn = entry & ((1ULL << 55) - 1); /* bits 0..54 = PFN */
    return (pfn << 12) | (vaddr & (pagesize - 1));
}

int main(void)
{
    pagesize = sysconf(_SC_PAGESIZE);

    /* Put x on its own dedicated page, so writing *x only affects x. */
    volatile int *x = mmap(NULL, (size_t)pagesize,
                           PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (x == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    *x = 1;

    fflush(stdout);
    pid_t pid = fork();

    if (pid == 0) {
        printf("[child ] pid=%d BEFORE write: x=%d  &x=%p  phys=0x%llx\n",
               getpid(), *x, (void *)x, (unsigned long long)phys_addr(x));
        *x += 1;   /* write -> copy-on-write */
        printf("[child ] pid=%d AFTER  write: x=%d  &x=%p  phys=0x%llx\n",
               getpid(), *x, (void *)x, (unsigned long long)phys_addr(x));
        _exit(0);
    }

    printf("[parent] pid=%d BEFORE write: x=%d  &x=%p  phys=0x%llx\n",
           getpid(), *x, (void *)x, (unsigned long long)phys_addr(x));
    *x -= 1;       /* write -> copy-on-write */
    printf("[parent] pid=%d AFTER  write: x=%d  &x=%p  phys=0x%llx\n",
           getpid(), *x, (void *)x, (unsigned long long)phys_addr(x));
    wait(NULL);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/** 
 * @brief Read exactly n bytes from a file descriptor
 * @param fd File descriptor
 * @param usrbuf Buffer to store the read data
 * @param n Number of bytes to read
 * @return Number of bytes read, or -1 on error
 * @author chendou
 * @date 2024-06-20 15:00:00        
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n){
    size_t nleft = n;// Number of bytes left to read
    ssize_t nread;// Number of bytes read in one read() call
    char *bufp = usrbuf;// Pointer to the buffer to store the read data
    // Read n bytes from the file descriptor fd
    while (nleft > 0) {// While there are still bytes left to read  
        if ((nread = read(fd, bufp, nleft)) < 0) {// If read() returns an error
            if (errno == EINTR) /* interrupted by sig handler return */
                nread = 0;      /* and call read() again */
            else
                return -1;      /* errno set by read() */
        }
        else if (nread == 0)// If read() returns 0, it means EOF
            break;              /* EOF */
        nleft -= nread;// Decrease the number of bytes left to read
        bufp += nread; // Move the buffer pointer forward
    }
    return (n - nleft);         /* Return >= 0 */  // Return the number of bytes read
}

/** 
 * @brief Write exactly n bytes to a file descriptor
 * @param fd File descriptor
 * @param usrbuf Buffer to write
 * @param n Number of bytes to write
 * @return Number of bytes written, or -1 on error
 * @author chendou
 * @date 2024-06-20 15:00:00        
 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n){
    size_t nleft = n;// Number of bytes left to write
    ssize_t nwritten;// Number of bytes written in one write() call
    char *bufp = usrbuf;// Write n bytes to the file descriptor fd
    // Write n bytes to the file descriptor fd
    while (nleft > 0) {// While there are still bytes left to write
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {// If write() returns an error
            if (errno == EINTR)  /* Interrupted by sig handler return */
                nwritten = 0;   /* and call write() again */
            else
                return -1;      /* errno set by write() */
        }
        nleft -= nwritten;// Decrease the number of bytes left to write
        bufp += nwritten;// Move the buffer pointer forward 
    }
    return n;// Return the number of bytes written
}
// rio_t structure
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;// File descriptor
    int rio_cnt;// Number of bytes left to read
    char *rio_bufptr;// Pointer to the buffer to store the read data
    char rio_buf[RIO_BUFSIZE];
} rio_t;
/**
 * @brief Initialize the rio_t structure
 * @param rp Pointer to the rio_t structure
 * @param fd File descriptor
 * @author chendou
 * @date 2024-06-20 15:00:00        
 */
void rio_readinitb(rio_t *rp, int fd){
    rp->rio_fd = fd;// Set the file descriptor
    rp->rio_cnt = 0;// Set the number of bytes left to read to 0
    rp->rio_bufptr = rp->rio_buf;// Set the pointer to the buffer to store the read data
}

/** 
 * @brief Read n bytes from a file descriptor
 * @param rp Pointer to the rio_t structure
 * @param usrbuf Buffer to store the read data
 * @param n Number of bytes to read
 * @return Number of bytes read, or -1 on error
 * @author chendou
 * @date 2024-06-20 15:00:00        
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    int cnt;
    // Read n bytes from the file descriptor fd
    while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
        // Refill the buffer if it is empty
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf,
                           sizeof(rp->rio_buf));
        // If read() returns an error
        if (rp->rio_cnt < 0){ 
            if (errno != EINTR) /* Interrupted by sig handler return */
                return -1;
        }
        else if (rp->rio_cnt == 0)  /* EOF */
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    // Copy the minimum of n bytes and rp->rio_cnt bytes from the internal buffer to the user buffer
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

/**
 * @brief Read a line from a file descriptor
 * @param rp Pointer to the rio_t structure
 * @param usrbuf Buffer to store the read data
 * @param maxlen Maximum number of bytes to read
 * @return Number of bytes read, or -1 on error
 * @author chendou
 * @date 2024-06-20 15:00:00        
 */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{

    int n, rc;// Number of bytes read, return value of rio_read()
    char c, *bufp = usrbuf;// Pointer to the buffer to store the read data
    
    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {// If rio_read() returns 1, it means it read 1 byte
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {// If rio_read() returns 0, it means EOF
            if (n == 1)
                return 0; /* EOF, no data read */
            else
                break;    /* EOF, some data was read */
        } else
            return -1;  /* Error */
    }
    *bufp = 0;
    return n-1;
}
/**
 * @brief Read n bytes from a file descriptor
 * @param rp Pointer to the rio_t structure
 * @param usrbuf Buffer to store the read data
 * @param n Number of bytes to read
 * @return Number of bytes read, or -1 on error
 * @author chendou
 * @date 2024-06-20 15:00:00        
 */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0)
            return -1;          /* errno set by read() */
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);         /* Return >= 0 */
}

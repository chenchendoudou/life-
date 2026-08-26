#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
 * 演示：同一个进程，两次open同一个文件，
 * 两个描述符指向不同的文件表项，文件位置各自独立
 */
int main() {
    // 先准备一个测试文件
    int tmp_fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(tmp_fd, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
    close(tmp_fd);

    // 两次打开同一个文件
    int fd1 = open("test.txt", O_RDONLY);
    int fd2 = open("test.txt", O_RDONLY);
    
    printf("fd1 = %d, fd2 = %d\n\n", fd1, fd2);

    char buf[10];

    // 用 fd1 读 5 个字节
    memset(buf, 0, sizeof(buf));
    read(fd1, buf, 5);
    printf("fd1 读了5个字节: %s\n", buf);
    printf("  → 此时 fd1 的文件位置在 5\n\n");

    // 用 fd2 读 5 个字节 —— 它从自己的位置 0 开始读！
    memset(buf, 0, sizeof(buf));
    read(fd2, buf, 5);
    printf("fd2 读了5个字节: %s\n", buf);
    printf("  → fd2 的文件位置独立于 fd1，仍然从 0 开始！\n\n");

    // 再用 fd1 读 5 个字节 —— 从位置 5 继续
    memset(buf, 0, sizeof(buf));
    read(fd1, buf, 5);
    printf("fd1 又读了5个字节: %s\n", buf);
    printf("  → fd1 接着上次的位置 5 继续读\n");

    close(fd1);
    close(fd2);
    return 0;
}
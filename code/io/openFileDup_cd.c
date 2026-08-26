#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * 演示：dup2让两个描述符指向同一个文件表项
 * → 它们共享文件位置
 */
int main() {
    int fd1 = open("test5.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    write(fd1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
    close(fd1);

    int fd = open("test5.txt", O_RDWR);
    // fd2 指向和 fd 同一个文件表项
    int fd2 = dup(fd);
    // 或者用：dup2(fd, 10); // 让描述符10也指向同一个文件表项

    printf("fd = %d, fd2 = %d\n", fd, fd2);
    printf("它们指向同一个文件表项，共享文件位置\n\n");

    char buf[10];

    // 用 fd 读5个字节
    read(fd, buf, 5);
    buf[5] = '\0';
    printf("fd  读了5个字节: %s\n", buf);

    // 用 fd2 读5个字节 —— 从 fd 读完之后的位置继续！
    read(fd2, buf, 5);
    buf[5] = '\0';
    printf("fd2 读了5个字节: %s\n", buf);
    printf("  → fd2 从 fd 读完之后的位置继续！\n");
    printf("  → 证明 dup 让两个描述符共享同一个文件位置！\n");

    close(fd);
    close(fd2);
    return 0;
}
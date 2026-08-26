#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
 * 演示：fork之后，父子进程共享同一个文件表项，
 * 因此共享文件位置
 */
int main() {
    // 准备测试文件
    int tmp_fd = open("test2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(tmp_fd, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
    close(tmp_fd);

    // 父进程打开文件
    int fd = open("test2.txt", O_RDONLY);
    printf("父进程: fd = %d\n", fd);

    pid_t pid = fork();

    if (pid == 0) {
        // ---------- 子进程 ----------
        char buf[10];
        memset(buf, 0, sizeof(buf));
        read(fd, buf, 5);
        printf("子进程: 读了5个字节: %s\n", buf);
        // 注意：这里没有close，后面父进程会看到位置变化
        close(fd);
        return 0;
    } else {
        // ---------- 父进程 ----------
        // 先让子进程先执行，确保它先读
        wait(NULL);

        // 父进程现在来读 —— 会从子进程读完的位置继续！
        char buf[10];
        memset(buf, 0, sizeof(buf));
        read(fd, buf, 5);
        printf("父进程: 读了5个字节: %s\n", buf);
        printf("  → 父进程接着子进程的位置 5 继续读！\n");
        printf("  → 证明父子进程共享同一个文件位置！\n");

        close(fd);
    }

    return 0;
}
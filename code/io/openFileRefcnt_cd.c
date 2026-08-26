#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * 演示：fork之后，父进程和子进程都必须close，
 * 引用计数归零，文件才会真正关闭
 */
int main() {
    // 以读写方式打开文件
    int fd = open("test4.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    // 写点内容进去
    write(fd, "HELLO", 5);

    pid_t pid = fork();

    if (pid == 0) {
        // 子进程
        printf("子进程: 关闭描述符\n");
        close(fd);   // refcnt: 2 → 1
        return 0;
    } else {
        // 父进程等待子进程结束
        wait(NULL);

        printf("父进程: 此时refcnt = 1（子进程已关，但父进程还没关）\n");
        printf("父进程: 尝试读文件...\n");
        
        // 注意：这里用 lseek 读取，文件位置还是0
        // 因为在fork之前写入了"HELLO"且没有移动位置
        // 注意我们写入了5个字节，然后没有移动位置
        // 但这里我们测试文件是否仍然可用
        char buf[10];
        lseek(fd, 0, SEEK_SET);  // 回到开头
        read(fd, buf, 5);
        buf[5] = '\0';
        printf("父进程: 读到了: %s\n", buf);
        printf("父进程: 仍然可以操作文件！因为refcnt还没到0\n");

        // 现在父进程关闭
        close(fd);   // refcnt: 1 → 0，文件表项被释放
        printf("父进程: 现在refcnt = 0，文件表项被释放\n");
    }

    return 0;
}
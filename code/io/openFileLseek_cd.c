#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

// 辅助函数：打印当前文件位置
void print_pos(int fd, const char* label) {
    off_t pos = lseek(fd, 0, SEEK_CUR);
    printf("%s: 文件位置 = %ld\n", label, pos);
}

/*
 * 演示：用lseek查看文件位置，验证两次open的文件位置独立
 */
int main() {
    // 准备文件
    int tmp = open("test3.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(tmp, "1234567890", 10);
    close(tmp);

    int fd1 = open("test3.txt", O_RDONLY);
    int fd2 = open("test3.txt", O_RDONLY);

    print_pos(fd1, "fd1 初始位置");
    print_pos(fd2, "fd2 初始位置");
    printf("\n");

    // fd1 读3个字节
    char buf[10];
    read(fd1, buf, 3);
    printf("fd1 读了3个字节\n");
    print_pos(fd1, "fd1 现在位置");
    print_pos(fd2, "fd2 现在位置 (完全不受影响)");
    printf("\n");

    // fd2 读3个字节 —— 从自己的0位置开始
    read(fd2, buf, 3);
    printf("fd2 读了3个字节\n");
    print_pos(fd1, "fd1 现在位置 (不受影响)");
    print_pos(fd2, "fd2 现在位置");

    close(fd1);
    close(fd2);
    return 0;
}
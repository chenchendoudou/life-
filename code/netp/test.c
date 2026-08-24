#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(void)
{
    /* ===== 1. inet_pton: IP字符串 → 网络字节序二进制 ===== */
    printf("===== 1. inet_pton 转换 =====\n");

    const char *ip = "192.168.1.100";
    struct in_addr addr;

    int ret = inet_pton(AF_INET, ip, &addr);
    printf("inet_pton(\"%s\") 返回: %d (1=成功)\n", ip, ret);

    /* 打印二进制内容 (网络字节序 = 大端) */
    unsigned char *p = (unsigned char *)&addr;
    printf("二进制(网络字节序): %02X %02X %02X %02X\n",
           p[0], p[1], p[2], p[3]);
    printf("  含义:   192  168    1  100\n\n");

    /* ===== 2. inet_ntop: 网络字节序二进制 → IP字符串 ===== */
    printf("===== 2. inet_ntop 转换 =====\n");

    /* 构造一个网络字节序的 IPv4 地址 */
    struct in_addr addr2;
    memset(&addr2, 0, sizeof(addr2));

    /* 方法: 用 inet_pton 构造 (推荐) */
    inet_pton(AF_INET, "198.168.1.100", &addr2);

    char buf[INET_ADDRSTRLEN];
    const char *result = inet_ntop(AF_INET, &addr2, buf, sizeof(buf));
    printf("inet_ntop 返回: %p (成功=buf指针)\n", (void *)result);
    printf("inet_ntop 结果: %s\n", buf);

    /* 验证: pton 和 ntop 互逆 */
    char buf2[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, buf2, sizeof(buf2));
    printf("验证互逆: %s → %s (相同? %s)\n", ip, buf2,
           strcmp(ip, buf2) == 0 ? "是" : "否");

    return 0;
}
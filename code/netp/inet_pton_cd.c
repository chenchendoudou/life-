#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
//gcc inet_pton_cd.c -o inet_pton_cd
/*
 * inet_pton / inet_ntop —— IP 地址字符串与二进制的互转
 *
 * 函数原型:
 *   int inet_pton(int af, const char *src, void *dst);
 *     作用: 字符串 → 二进制(网络字节序)
 *     返回: 1成功, 0字符串无效, -1不支持的af
 *
 *   const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
 *     作用: 二进制(网络字节序) → 字符串
 *     返回: 成功返回dst指针, 失败返回NULL
 *
 * 头文件: <arpa/inet.h>
 */

/* 辅助函数：打印IPv4二进制内容 */
void print_ipv4_binary(const char *label, struct in_addr *addr)
{
    unsigned char *p = (unsigned char *)addr;
    printf("%s: %02X %02X %02X %02X\n", label, p[0], p[1], p[2], p[3]);
}

int main(void)
{
    /* ===== 例子1: inet_pton 基础用法 ===== */
    printf("===== 例子1: inet_pton 基础用法 =====\n\n");

    struct in_addr addr1;

    /* "192.168.1.1" → 网络字节序二进制 */
    int ret = inet_pton(AF_INET, "192.168.1.1", &addr1);
    printf("inet_pton(\"192.168.1.1\") 返回: %d\n", ret);
    print_ipv4_binary("  二进制结果", &addr1);
    printf("  (网络字节序: 192 168 1 1 → C0 A8 01 01)\n\n");

    /* ===== 例子2: inet_ntop 基础用法 ===== */
    printf("===== 例子2: inet_ntop 基础用法 =====\n\n");

    char ip_str[INET_ADDRSTRLEN];

    /* 网络字节序二进制 → 点分十进制字符串 */
    const char *result = inet_ntop(AF_INET, &addr1, ip_str, sizeof(ip_str));
    printf("inet_ntop 结果: %s\n", ip_str);
    printf("  返回值与ip_str相同: %s\n", result == ip_str ? "是" : "否");
    printf("  (INET_ADDRSTRLEN = %d, 足够存放 \"255.255.255.255\" + '\\0')\n\n",
           INET_ADDRSTRLEN);

    /* ===== 例子3: 常用IP地址转换 ===== */
    printf("===== 例子3: 常用IP地址转换 =====\n\n");

    const char *ips[] = {
        "0.0.0.0",
        "127.0.0.1",
        "192.168.1.100",
        "255.255.255.255",
        NULL
    };

    for (int i = 0; ips[i] != NULL; i++) {
        struct in_addr addr;
        inet_pton(AF_INET, ips[i], &addr);

        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr, buf, sizeof(buf));

        unsigned char *p = (unsigned char *)&addr;
        printf("  %-18s → [ %02X %02X %02X %02X ] → %s\n",
               ips[i], p[0], p[1], p[2], p[3], buf);
    }
    printf("\n");

    /* ===== 例子4: 错误处理 ===== */
    printf("===== 例子4: 错误处理 =====\n\n");

    struct in_addr bad;
    char small[5];

    ret = inet_pton(AF_INET, "999.999.999.999", &bad);
    printf("无效IP \"999.999.999.999\": ret=%d (0=字符串无效)\n", ret);

    ret = inet_pton(AF_INET, "", &bad);
    printf("空字符串: ret=%d\n", ret);

    result = inet_ntop(AF_INET, &addr1, small, sizeof(small));
    printf("缓冲区太小(5字节): result=%p (NULL=缓冲不足)\n", (void *)result);
    printf("\n");

    /* ===== 例子5: 实际构建 socket 地址结构 ===== */
    printf("===== 例子5: 构建 socket 地址结构 =====\n\n");

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    /* 用户输入的IP → 存入sockaddr_in */
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) != 1) {
        fprintf(stderr, "IP转换失败\n");
        exit(1);
    }

    /* 调试时把二进制地址打回字符串 */
    char debug[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server.sin_addr, debug, sizeof(debug));
    printf("服务器地址: %s:%d\n", debug, ntohs(server.sin_port));
    printf("  sin_family = AF_INET (%d)\n", server.sin_family);
    printf("  sin_port   = %d (网络字节序: %04X)\n",
           ntohs(server.sin_port), server.sin_port);
    print_ipv4_binary("  sin_addr  ", &server.sin_addr);
    printf("\n");

    /* ===== 例子6: IPv6 支持 ===== */
    printf("===== 例子6: IPv6 支持 =====\n\n");

    struct in6_addr ipv6;
    ret = inet_pton(AF_INET6, "2001:db8::1", &ipv6);
    printf("inet_pton(\"2001:db8::1\"): ret=%d\n", ret);

    char ipv6_str[INET6_ADDRSTRLEN];
    result = inet_ntop(AF_INET6, &ipv6, ipv6_str, sizeof(ipv6_str));
    printf("inet_ntop 结果: %s\n", ipv6_str);
    printf("  (INET6_ADDRSTRLEN = %d)\n\n", INET6_ADDRSTRLEN);

    /* ===== 例子7: 与旧函数对比 ===== */
    printf("===== 例子7: 与旧函数对比 =====\n\n");

    /* 旧函数 inet_addr (已废弃) */
    unsigned long old = inet_addr("127.0.0.1");
    printf("inet_addr(\"127.0.0.1\") = %lu (主机字节序, 已废弃)\n", old);

    /* 新函数 inet_pton */
    struct in_addr addr_new;
    inet_pton(AF_INET, "127.0.0.1", &addr_new);
    printf("inet_pton(\"127.0.0.1\")  = %u (网络字节序, 推荐)\n",
           addr_new.s_addr);

    printf("\n===== 总结 =====\n");
    printf("inet_pton: 人读字符串 → 网络传输二进制 (发送前用)\n");
    printf("inet_ntop: 网络传输二进制 → 人读字符串 (调试时用)\n");
    printf("两者都支持 IPv4/IPv6, 是 inet_addr/inet_ntoa 的替代品\n");

    return 0;
}
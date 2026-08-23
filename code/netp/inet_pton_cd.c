#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/*
 * inet_pton 和 inet_ntop 函数讲解
 *
 * 这两个函数用于 IP 地址在 网络字节序二进制格式 和 点分十进制字符串格式 之间的转换。
 * 它们是比 inet_addr / inet_ntoa 更现代、更通用的函数，支持 IPv4 和 IPv6。
 *
 * 头文件: <arpa/inet.h>  (Linux), 或 <arpa/inet.h> + <netinet/in.h>
 *
 * int inet_pton(int af, const char *src, void *dst);
 *   - 将字符串格式(src) 转换为 网络字节序二进制格式(dst)
 *   - af: 地址族，AF_INET(IPv4) 或 AF_INET6(IPv6)
 *   - src: 输入的 IP 字符串，如 "192.168.1.1"
 *   - dst: 输出的二进制地址缓冲区(通常是 struct in_addr 或 struct in6_addr)
 *   - 返回值: 1 成功, 0 字符串无效, -1 错误(af 不支持)
 *
 * const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
 *   - 将网络字节序二进制格式(src) 转换为 字符串格式(dst)
 *   - af: 地址族，AF_INET(IPv4) 或 AF_INET6(IPv6)
 *   - src: 输入的二进制地址
 *   - dst: 输出的字符串缓冲区
 *   - size: dst 缓冲区大小，IPv4 至少 16 字节(INET_ADDRSTRLEN)，IPv6 至少 46 字节(INET6_ADDRSTRLEN)
 *   - 返回值: 成功返回 dst 指针，失败返回 NULL
 */

int main()
{
    /* ========== 1. inet_pton: 字符串 -> 二进制 ========== */
    printf("====== 1. inet_pton: 字符串 IP  ->  二进制网络字节序\n");

    struct in_addr ipv4_addr;
    const char *ipv4_str = "www.baidu.com"; /* 也可以使用 "192.168.1.100" */

    int ret = inet_pton(AF_INET, ipv4_str, &ipv4_addr);
    if (ret == 1)
        printf("  IPv4 转换成功: \"%s\"\n", ipv4_str);
    else if (ret == 0)
        printf("  IPv4 转换失败: 字符串无效 \"%s\"\n", ipv4_str);
    else
        printf("  IPv4 转换失败: af 不支持\n");

    /* 查看二进制内容(以十六进制打印网络字节序) */
    unsigned char *p = (unsigned char *)&ipv4_addr;
    printf("  二进制网络字节序(十六进制): %02x %02x %02x %02x\n",
           p[0], p[1], p[2], p[3]);

    /* ========== 2. inet_ntop: 二进制 -> 字符串 ========== */
    printf("\n====== 2. inet_ntop: 二进制网络字节序  ->  字符串 IP\n");

    char ip_str[INET_ADDRSTRLEN]; /* INET_ADDRSTRLEN = 16 */
    const char *result = inet_ntop(AF_INET, &ipv4_addr, ip_str, sizeof(ip_str));
    if (result != NULL)
        printf("  转换结果: %s\n", ip_str);
    else
        printf("  转换失败\n");

    /* ========== 3. 演示 IPv6 ========== */
    printf("\n====== 3. IPv6 转换演示\n");

    struct in6_addr ipv6_addr;
    const char *ipv6_str = "2001:db8::1";

    ret = inet_pton(AF_INET6, ipv6_str, &ipv6_addr);
    if (ret == 1)
        printf("  IPv6 转换成功: \"%s\"\n", ipv6_str);
    else
        printf("  IPv6 转换失败, ret=%d\n", ret);

    char ipv6_str_buf[INET6_ADDRSTRLEN]; /* INET6_ADDRSTRLEN = 46 */
    result = inet_ntop(AF_INET6, &ipv6_addr, ipv6_str_buf, sizeof(ipv6_str_buf));
    if (result != NULL)
        printf("  转换回字符串: %s\n", ipv6_str_buf);
    else
        printf("  转换失败\n");

    /* ========== 4. 演示错误处理 ========== */
    printf("\n====== 4. 错误处理演示\n");

    /* 无效的 IPv4 字符串 */
    struct in_addr bad_addr;
    ret = inet_pton(AF_INET, "999.999.999.999", &bad_addr);
    printf("  inet_pton(\"999.999.999.999\") 返回: %d (0 表示字符串无效)\n", ret);

    /* 缓冲区太小 */
    char small_buf[5];
    result = inet_ntop(AF_INET, &ipv4_addr, small_buf, sizeof(small_buf));
    printf("  inet_ntop 缓冲区太小(%zu字节) 返回: %p (NULL表示缓冲不够)\n",
           sizeof(small_buf), (void *)result);

    /* ========== 5. 实际应用: 将 IP 字符串存入 socket 结构体 ========== */
    printf("\n====== 5. 实际应用: 构建 socket 地址结构\n");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    const char *server_ip = "127.0.0.1";
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) != 1)
    {
        fprintf(stderr, "  无效的 IP 地址: %s\n", server_ip);
        exit(EXIT_FAILURE);
    }

    char debug_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server_addr.sin_addr, debug_ip, sizeof(debug_ip));
    printf("  sockaddr_in: %s:%d\n", debug_ip, ntohs(server_addr.sin_port));

    return 0;
}

/*
 * 编译运行:
 *   gcc inet_pton.c -o inet_pton
 *   ./inet_pton
 *
 * 关键要点总结:
 * 1. inet_pton: 字符串 -> 二进制网络字节序, 用于接收用户输入的 IP
 * 2. inet_ntop: 二进制网络字节序 -> 字符串, 用于调试日志打印 IP
 * 3. 这两个函数支持 IPv4 和 IPv6, 可替代旧的 inet_addr/inet_ntoa
 * 4. 二进制格式就是网络传输用的字节序, 不是主机字节序
 */
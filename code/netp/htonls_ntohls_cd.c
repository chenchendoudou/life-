#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
//cd /home/xj/chendou/life-/code/netp && gcc htonls_ntohls_cd.c -o htonls_ntohls_cd && ./htonls_ntohls_cd
/* 
 * 字节序转换函数：
 *   htons → host to network short    (16位，用于端口号)
 *   htonl → host to network long     (32位，用于IP地址)
 *   ntohs → network to host short    (16位)
 *   ntohl → network to host long     (32位)
 *
 * 网络字节序 = 大端序 (Big-Endian)
 * 主机字节序 = 取决于CPU架构（x86/x64为小端序 Little-Endian）
 *
 * 记忆技巧：
 *   h = host（主机）  n = network（网络）  s = short（16位）  l = long（32位）
 *   to = to（到）     h = host           s = short          l = long
 */

/* 判断主机是大端还是小端 */
void check_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } u;
    u.i = 0x01020304;
    if (u.c[0] == 1)
        printf("主机字节序: 大端序 (Big-Endian)\n");
    else
        printf("主机字节序: 小端序 (Little-Endian)\n");
}

/* 打印16位数值的字节排列 */
void print_bytes_16(uint16_t val, const char *label)
{
    unsigned char *p = (unsigned char *)&val;
    printf("%s: 0x%04X → ", label, val);
    printf("[ %02X %02X ]\n", p[0], p[1]);
}

/* 打印32位数值的字节排列 */
void print_bytes_32(uint32_t val, const char *label)
{
    unsigned char *p = (unsigned char *)&val;
    printf("%s: 0x%08X → ", label, val);
    printf("[ %02X %02X %02X %02X ]\n", p[0], p[1], p[2], p[3]);
}

int main(void)
{
    printf("====== 字节序转换函数详解 ======\n\n");

    check_endian();
    printf("\n");

    /* 
     * 1. htons / ntohs —— 16位短整型转换
     *    通常用于端口号（0~65535）
     */
    printf("--- 16位转换 (htons / ntohs) ---\n\n");

    uint16_t port_host = 0x1234;// 主机序端口号 电脑上是小端序

    uint16_t port_net = htons(port_host);// 转换为网络序     电脑上是大端序

    uint16_t port_back = ntohs(port_net);// 转换回来为主机序

    print_bytes_16(port_host,   "主机序 (port_host) ");
    print_bytes_16(port_net,    "网络序 (htons)     ");
    print_bytes_16(port_back,   "转换回来 (ntohs)   ");

    printf("htons(%d) = %d  →  用于发送端口号\n", port_host, port_net);
    printf("ntohs(%d) = %d  →  用于接收端口号\n", port_net, port_back);
    printf("\n");

    /*
     * 2. htonl / ntohl —— 32位长整型转换
     *    通常用于IP地址（如 192.168.1.1）
     */
    printf("--- 32位转换 (htonl / ntohl) ---\n\n");

    /* 
     * IP地址 192.168.1.1 的主机序表示:
     *   点分十进制: 192 . 168 . 1 . 1
     *   主机序 (小端): 0x0101A8C0
     *   网络序 (大端): 0xC0A80101
     */
    uint32_t ip_host = (192u << 24) | (168u << 16) | (1u << 8) | 1u;
    uint32_t ip_net = htonl(ip_host);
    uint32_t ip_back = ntohl(ip_net);

    print_bytes_32(ip_host,    "主机序 (ip_host)   ");
    print_bytes_32(ip_net,     "网络序 (htonl)     ");
    print_bytes_32(ip_back,    "转换回来 (ntohl)   ");

    printf("htonl(192.168.1.1) 主机序 → 网络序\n");
    printf("ntohl(...) 网络序 → 主机序\n");
    printf("\n");

    /* 
     * 3. 实际使用：inet_pton / inet_ntop
     *    这两个函数内部已经处理了字节序转换
     *    但需要知道它们操作的是网络序
     */
    printf("--- 实际使用演示 ---\n\n");

    struct sockaddr_in addr;

    /* 
     * inet_pton: 将点分十进制字符串转为网络序二进制
     * inet_ntop: 将网络序二进制转为点分十进制字符串
     */
    inet_pton(AF_INET, "192.168.1.100", &addr.sin_addr);
    printf("inet_pton(\"192.168.1.100\"):\n");
    print_bytes_32(addr.sin_addr.s_addr, "  存储值(网络序)    ");
    printf("  用 ntohl 转回主机序: ");
    print_bytes_32(ntohl(addr.sin_addr.s_addr), "");

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));
    printf("  inet_ntop 转回字符串: %s\n\n", ip_str);

    /* 
     * 4. 常用常量
     */
    printf("--- 常用常量 ---\n\n");
    printf("INADDR_ANY:       %u (0.0.0.0, 绑定所有网卡)\n", INADDR_ANY);
    printf("INADDR_LOOPBACK:  %u (127.0.0.1, 回环地址)\n", INADDR_LOOPBACK);
    printf("INADDR_BROADCAST: %u (255.255.255.255, 广播)\n", INADDR_BROADCAST);
    printf("\n");

    /* 
     * 5. 字节序转换的本质：swap bytes
     *    在小端机器上，这些函数就是简单的字节交换
     */
    printf("--- 本质：字节交换 ---\n\n");
    printf("htons 等价于: ((val & 0xFF) << 8) | ((val >> 8) & 0xFF)\n");
    printf("htonl 等价于: 4个字节的逆序排列\n");
    printf("\n");

    printf("====== 总结 ======\n");
    printf("1. 网络传输一律使用大端序 (Network Byte Order)\n");
    printf("2. 主机内部使用主机字节序 (Host Byte Order)\n");
    printf("3. 发送前用 htons/htonl 转换，接收后用 ntohs/ntohl 还原\n");
    printf("4. 端口号用 16位转换，IP地址用 32位转换\n");

    return 0;
}
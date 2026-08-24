#include "csapp.h"
//test
int main(int argc, char **argv)
{
    struct addrinfo *p, *listp, hints;
    //              ↑       ↑        ↑
    //            遍历指针  结果链表  过滤条件(暗示)

    char buf[MAXLINE];// 存放 IP 字符串的缓冲区
    int rc, flags;// 返回码、控制标志

    if (argc != 2) {
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]);
        exit(0);
    }

    /* Get a list of addrinfo records */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;          /* 只要IPv4 */
    hints.ai_socktype = SOCK_STREAM;  /* 只要TCP连接 */
    if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
        exit(1);
    }

    /* Walk the list and display each IP address */
    flags = NI_NUMERICHOST;  /* 显示数字IP，不查域名 */
    for (p = listp; p; p = p->ai_next) {
        getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
        printf("aaaaa   %s\n", buf);
    }

    freeaddrinfo(listp);
    exit(0);
}
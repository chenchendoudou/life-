#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  
//dd2hex = dotted decimal to hex
//点分十进制IP  →  十六进制

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <dotted_decimal_ip>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ip_str = argv[1];
    struct in_addr addr;

    if (inet_pton(AF_INET, ip_str, &addr) != 1) {
        fprintf(stderr, "Invalid IP address: %s\n", ip_str);
        return EXIT_FAILURE;
    }

    unsigned char *p = (unsigned char *)&addr;
    printf("Dotted decimal IP: %s\n", ip_str);
    printf("Converted hex: 0x%02X%02X%02X%02X\n", p[0], p[1], p[2], p[3]);

    return EXIT_SUCCESS;
}
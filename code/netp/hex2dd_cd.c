#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>  
//hex2dd = hex to dotted decimal
//十六进制  →  点分十进制IP
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hex_string>\n", argv[0]);
        return EXIT_FAILURE;
    }
    //argv[1]是0x开头的十六进制字符串

    const char *hex_str = argv[1];
    unsigned char bytes[4];
    int ret = sscanf(hex_str, "0x%2hhx%2hhx%2hhx%2hhx", &bytes[0], &bytes[1], &bytes[2], &bytes[3]);
   
    if (ret != 4) {
        fprintf(stderr, "Invalid hex string: %s\n", hex_str);
        return EXIT_FAILURE;
    }

    struct in_addr addr;
    memcpy(&addr, bytes, sizeof(addr));

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &addr, ip_str, sizeof(ip_str)) == NULL) {
        perror("inet_ntop");
        return EXIT_FAILURE;
    }

    printf("Hex string: %s\n", hex_str);
    printf("Converted IP address: %s\n", ip_str);

    return EXIT_SUCCESS;
}

#include <stdio.h>
int main()
{
    int i = 0x7fffffff;
        printf("%d\n", i);
    for(; i > 0; i++) {
        printf("%d\n", i);
    }
    
    printf("%d\n", i);
    return 0;
}
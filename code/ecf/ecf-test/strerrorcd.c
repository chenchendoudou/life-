    #include <stdio.h>
    #include <string.h>     

int main()
{
    // linux errno最大一般不超过140，循环打印
    for(int i = 0; i < 140; i++)
    {
        printf("errno=%3d | %s\n", i, strerror(i));
    }
    return 0;
}
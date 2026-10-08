/**
 * @file exitcd.c
 * @brief 一个微试验：把 exit() 的返回值喂给 strerror() 会怎样。
 * @details
 *   这是为了直观理解 `exit(int status)` 和 `strerror(int errnum)`
 *   在参数/返回类型上的差别而写的小试验：
 *
 *   @code
 *     printf("exitcd %s\n", strerror(exit(1)));
 *   @endcode
 *
 *   注意：
 *     - exit(1) 不会返回（控制流不会回到这条语句之后），
 *       因此严格意义上 printf 这行是不可达的，编译器可能会警告
 *     - exit 是 void 函数（C99/C11 起），即便用 GNU 扩展允许表达式上下文，
 *       `strerror(exit(1))` 也只是演示"类型上能编译过"
 *     - 真正的退出码 1 由 exit 自带的语义传递，不需要 strerror
 *
 *   该文件**没有**被 Makefile 收录，仅作临时试验用。
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/**
 * @brief 进程入口：触发 exit(1) 让进程以状态码 1 退出。
 * @return 不会返回（exit 在内部 _exit）。
 */
void main()
{
    printf("exitcd %s\n", strerror(exit(1)));
}
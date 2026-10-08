/*
 * demo_setcc.c
 *
 * 演示有符号/无符号比较如何被编译器翻译成 cmp + SET 指令。
 *
 * 目标是亲手用编译器把下面这些 C 表达式编译成汇编,
 * 看 -m32 或 objdump -d 输出的 cmp / sete / setl / setb 指令,
 * 把它们和 CF / ZF / SF / OF 这四个标志位的对应规则对上号。
 */

#include <stdio.h>

/* 用 volatile 防止编译器把整个函数优化掉, 保证每条 cmp+SET 都保留下来 */
int a_signed = -5, b_signed = 3;
unsigned a_unsigned = 1u, b_unsigned = 4u;

int main(void) {

    /* --- 有符号比较 --- */
    /* a < b  ->  setl  ->  SF ^ OF */
    volatile int r1 = (a_signed < b_signed);
    /* a == b ->  sete  ->  ZF */
    volatile int r2 = (a_signed == b_signed);

    /* --- 无符号比较 --- */
    /* a < b  ->  setb / seta  ->  CF */
    volatile int r3 = (a_unsigned < b_unsigned);
    volatile int r4 = (a_unsigned == b_unsigned);

    /* 让编译器看不到 r1..r4 的使用, 只能老老实实生成 cmp+SET */
    printf("signed lt=%d, signed eq=%d\n", r1, r2);
    printf("unsigned lt=%d, unsigned eq=%d\n", r3, r4);
    return 0;
}

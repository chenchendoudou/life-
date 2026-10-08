/**
 * @file setjmp.c
 * @brief setjmp / longjmp 非局部跳转：从深层嵌套函数跳回
 * @details
 *   setjmp 保存当前栈上下文到 buf。longjmp 跳回 setjmp 那个点，
 *   此时 setjmp 返回非 0 值（由 longjmp 的第二个参数指定）。
 *
 *   本例中 error1=0、error2=1，所以会从 bar() 里的 longjmp(buf, 2)
 *   跳回 main，rc==2，输出 "Detected an error2 condition in foo"。
 *
 * @note 参考 CS:APP §8.6
 */
/* $begin setjmp */
#include "csapp.h"

/** longjmp 跳回的栈上下文。*/
jmp_buf buf;

/** 错误标志 1（被 foo() 检查）。*/
int error1 = 0;
/** 错误标志 2（被 bar() 检查）。*/
int error2 = 1;

void foo(void), bar(void);

/**
 * @brief 主函数：setjmp 后根据 rc 判断从哪个 longjmp 跳回
 * @return 0（不会到达，调用 exit(0)）
 */
int main()
{
    int rc;

    rc = setjmp(buf);
    if (rc == 0)
	foo();
    else if (rc == 1)
	printf("Detected an error1 condition in foo\n");
    else if (rc == 2)
	printf("Detected an error2 condition in foo\n");
    else
	printf("Unknown error condition in foo\n");
    exit(0);
}

/**
 * @brief 深层嵌套函数 foo：检查 error1 后调用 bar()
 */
void foo(void)
{
    if (error1)
	longjmp(buf, 1);
    bar();
}

/**
 * @brief 深层嵌套函数 bar：检查 error2 后 longjmp(buf, 2) 跳回 main
 */
void bar(void)
{
    if (error2)
	longjmp(buf, 2);
}
/* $end setjmp */

STRTOL(3)                                                                             Linux Programmer's Manual                                                                             STRTOL(3)
STRTOL(3)                                                                              Linux 程序员手册                                                                              STRTOL(3)

NAME
名称
       strtol, strtoll, strtoq - convert a string to a long integer
       strtol, strtoll, strtoq - 将字符串转换为长整数

SYNOPSIS
语法
       #include <stdlib.h>

       long strtol(const char *nptr, char **endptr, int base);
       long strtol(const char *nptr, char **endptr, int base);

       long long strtoll(const char *nptr, char **endptr, int base);
       long long strtoll(const char *nptr, char **endptr, int base);

   Feature Test Macro Requirements for glibc (see feature_test_macros(7)):
   glibc 的特性测试宏要求（参见 feature_test_macros(7)）：

       strtoll():
           _ISOC99_SOURCE
               || /* Glibc versions <= 2.19: */ _SVID_SOURCE || _BSD_SOURCE

DESCRIPTION
描述
       The  strtol()  function converts the initial part of the string in nptr to a long integer value according to the given base, which must be between 2 and 36 inclusive, or be the special value
       0.
       strtol() 函数根据给定的基数（base）将 nptr 字符串的初始部分转换为长整数值。基数必须在 2 到 36 之间（包括 2 和 36），或者是特殊值 0。

       The string may begin with an arbitrary amount of white space (as determined by isspace(3)) followed by a single optional '+' or '-' sign.  If base is zero or 16, the string may then  include
       a "0x" or "0X" prefix, and the number will be read in base 16; otherwise, a zero base is taken as 10 (decimal) unless the next character is '0', in which case it is taken as 8 (octal).
       字符串可以以任意数量的空白字符（由 isspace(3) 确定）开头，后跟一个可选的 '+' 或 '-' 符号。如果基数为 0 或 16，字符串可以包含 "0x" 或 "0X" 前缀，数字将以 16 进制读取；否则，基数为 0 时视为 10（十进制），除非下一个字符是 '0'，此时视为 8（八进制）。

       The  remainder of the string is converted to a long value in the obvious manner, stopping at the first character which is not a valid digit in the given base.  (In bases above 10, the letter
       'A' in either uppercase or lowercase represents 10, 'B' represents 11, and so forth, with 'Z' representing 35.)
       字符串的其余部分按明显的方式转换为长整型值，在遇到第一个不是给定基数有效数字的字符时停止。（在大于 10 的基数中，字母 'A'（大小写均可）表示 10，'B' 表示 11，依此类推，'Z' 表示 35。）

       If endptr is not NULL, strtol() stores the address of the first invalid character in *endptr.  If there were no digits at all, strtol() stores the original value of nptr in *endptr (and  re‐
       turns 0).  In particular, if *nptr is not '\0' but **endptr is '\0' on return, the entire string is valid.
       如果 endptr 不为 NULL，strtol() 将第一个无效字符的地址存储在 *endptr 中。如果根本没有数字，strtol() 将 nptr 的原始值存储在 *endptr 中（并返回 0）。特别地，如果 *nptr 不是 '\0' 但返回时 **endptr 是 '\0'，则整个字符串都是有效的。

       The strtoll() function works just like the strtol() function but returns a long long integer value.
       strtoll() 函数的工作方式与 strtol() 完全相同，但返回 long long 整数值。

RETURN VALUE
返回值
       The strtol() function returns the result of the conversion, unless the value would underflow or overflow.  If an underflow occurs, strtol() returns LONG_MIN.  If an overflow occurs, strtol()
       returns LONG_MAX.  In both cases, errno is set to ERANGE.  Precisely the same holds for strtoll() (with LLONG_MIN and LLONG_MAX instead of LONG_MIN and LONG_MAX).
       strtol() 函数返回转换结果，除非值会下溢或上溢。如果发生下溢，strtol() 返回 LONG_MIN。如果发生上溢，strtol() 返回 LONG_MAX。在这两种情况下，errno 都被设置为 ERANGE。strtoll() 的行为完全相同（使用 LLONG_MIN 和 LLONG_MAX 代替 LONG_MIN 和 LONG_MAX）。

ERRORS
错误
       EINVAL (not in C99) The given base contains an unsupported value.
       EINVAL（不在 C99 中）给定的基数包含不支持的值。

       ERANGE The resulting value was out of range.
       ERANGE 结果值超出范围。

       The implementation may also set errno to EINVAL in case no conversion was performed (no digits seen, and 0 returned).
       如果未执行任何转换（未看到数字，返回 0），实现也可能将 errno 设置为 EINVAL。

ATTRIBUTES
属性
       For an explanation of the terms used in this section, see attributes(7).
       有关本节中使用的术语的解释，请参见 attributes(7)。

       ┌──────────────────────────────┬───────────────┬────────────────┐
       │Interface                     │ Attribute     │ Value          │
       │接口                          │ 属性          │ 值             │
       ├──────────────────────────────┼───────────────┼────────────────┤
       │strtol(), strtoll(), strtoq() │ Thread safety │ MT-Safe locale │
       │strtol(), strtoll(), strtoq() │ 线程安全      │ MT-Safe locale │
       └──────────────────────────────┴───────────────┴────────────────┘

CONFORMING TO
标准
       strtol(): POSIX.1-2001, POSIX.1-2008, C89, C99 SVr4, 4.3BSD.
       strtol(): POSIX.1-2001, POSIX.1-2008, C89, C99, SVr4, 4.3BSD.

       strtoll(): POSIX.1-2001, POSIX.1-2008, C99.
       strtoll(): POSIX.1-2001, POSIX.1-2008, C99.

NOTES
注意事项
       Since strtol() can legitimately return 0, LONG_MAX, or LONG_MIN (LLONG_MAX or LLONG_MIN for strtoll()) on both success and failure, the calling program should set errno to 0 before the call,
       and then determine if an error occurred by checking whether errno has a nonzero value after the call.
       由于 strtol() 在成功和失败时都可能合法地返回 0、LONG_MAX 或 LONG_MIN（strtoll() 返回 LLONG_MAX 或 LLONG_MIN），调用程序应在调用前将 errno 设置为 0，然后通过检查调用后 errno 是否为非零值来确定是否发生错误。

       According to POSIX.1, in locales other than "C" and "POSIX", these functions may accept other, implementation-defined numeric strings.
       根据 POSIX.1，在 "C" 和 "POSIX" 以外的区域设置中，这些函数可能接受其他实现定义的数字字符串。

       BSD also has
       BSD 还提供

           quad_t strtoq(const char *nptr, char **endptr, int base);
           quad_t strtoq(const char *nptr, char **endptr, int base);

       with completely analogous definition.  Depending on the wordsize of the current architecture, this may be equivalent to strtoll() or to strtol().
       具有完全类似的定义。根据当前架构的字长，这可能等同于 strtoll() 或 strtol()。

EXAMPLES
示例
       The program shown below demonstrates the use of strtol().  The first command-line argument specifies a string from which strtol() should parse a number.  The second (optional) argument spec‐
       ifies the base to be used for the conversion.  (This argument is converted to numeric form using atoi(3), a function that performs no error checking and has a  simpler  interface  than  str‐
       tol().)  Some examples of the results produced by this program are the following:
       下面显示的程序演示了 strtol() 的用法。第一个命令行参数指定一个字符串，strtol() 应从中解析一个数字。第二个（可选）参数指定转换使用的基数。（此参数使用 atoi(3) 转换为数字形式，该函数不执行错误检查，接口比 strtol() 更简单。）该程序产生的一些结果示例如下：

           $ ./a.out 123
           strtol() returned 123
           $ ./a.out '    123'
           strtol() returned 123
           $ ./a.out 123abc
           strtol() returned 123
           Further characters after number: "abc"
           $ ./a.out 123abc 55
           strtol: Invalid argument
           $ ./a.out ''
           No digits were found
           $ ./a.out 4000000000
           strtol: Numerical result out of range

   Program source
   程序源码

       #include <stdlib.h>
       #include <limits.h>
       #include <stdio.h>
       #include <errno.h>

       int
       main(int argc, char *argv[])
       {
           int base;
           char *endptr, *str;
           long val;

           if (argc < 2) {
               fprintf(stderr, "Usage: %s str [base]\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           str = argv[1];
           base = (argc > 2) ? atoi(argv[2]) : 0;

           errno = 0;    /* To distinguish success/failure after call */
           val = strtol(str, &endptr, base);

           /* Check for various possible errors */

           if (errno != 0) {
               perror("strtol");
               exit(EXIT_FAILURE);
           }

           if (endptr == str) {
               fprintf(stderr, "No digits were found\n");
               exit(EXIT_FAILURE);
           }

           /* If we got here, strtol() successfully parsed a number */

           printf("strtol() returned %ld\n", val);

           if (*endptr != '\0')        /* Not necessarily an error... */
               printf("Further characters after number: \"%s\"\n", endptr);

           exit(EXIT_SUCCESS);
       }

SEE ALSO
另见
       atof(3), atoi(3), atol(3), strtod(3), strtoimax(3), strtoul(3),

COLOPHON
版本说明
       This  page  is  part  of  release  5.10  of the Linux man-pages project.  A description of the project, information about reporting bugs, and the latest version of this page, can be found at
       https://www.kernel.org/doc/man-pages/.
       本页是 Linux man-pages 项目版本 5.10 的一部分。项目描述、报告错误的信息以及本页的最新版本可以在 https://www.kernel.org/doc/man-pages/ 找到。

GNU                                                                                           2020-11-01                                                                                    STRTOL(3)
GNU                                                                                           2020-11-01                                                                                    STRTOL(3)

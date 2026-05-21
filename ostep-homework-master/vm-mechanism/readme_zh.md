# Overview
# 概述

This program allows you to see how address translations are performed in a
system with base and bounds registers. As before, there are two steps to
running the program to test out your understanding of base and bounds. First,
run without the -c flag to generate a set of translations and see if you can
correctly perform the address translations yourself. Then, when done, run with
the -c flag to check your answers.
这个程序让你了解在使用基址-界限寄存器的系统中如何执行地址转换。
与之前一样，运行程序来测试你对基址-界限的理解需要两个步骤。
首先，不带 -c 标志运行程序来生成一组转换任务，看看你能否正确地
自己执行地址转换。然后，完成后，带 -c 标志运行程序来检查你的答案。

In this homework, we will assume a slightly different address space than our
canonical one with a heap and stack at opposite ends of the space. Rather, we
will assume that the address space has a code section, then a fixed-sized
(small) stack, and a heap that grows downward right after, looking something
like you see in the Figure below. In this configuration, there is only one
direction of growth, towards higher regions of the address space.
在本作业中，我们假设的地址空间与典型的堆和栈位于空间两端的地址
空间略有不同。相反，我们假设地址空间包含代码段，然后是一个固定大
小（较小）的栈，紧接着是向下增长的堆，看起来类似于下图所示。在这
种配置中，只有一个增长方向，即朝向地址空间的更高区域。

```sh
  -------------- 0KB
  |    Code    |
  -------------- 2KB
  |   Stack    |
  -------------- 4KB
  |    Heap    |
  |     |      |
  |     v      |
  -------------- 7KB
  |   (free)   |
  |     ...    |
```

In the figure, the bounds register would be set to 7~KB, as that represents
the end of the address space. References to any address within the bounds
would be considered legal; references above this value are out of bounds and
thus the hardware would raise an exception.
在图中，界限寄存器将被设置为 7 KB，因为这代表地址空间的末尾。
对界限内任何地址的引用都被认为是合法的；超过此值的引用将越界，
因此硬件会引发异常。

To run with the default flags, type relocation.py at the command line. The
result should be something like this:
要使用默认标志运行，在命令行输入 relocation.py。结果应该如下所示：

```sh
prompt> ./relocation.py 
...
Base-and-Bounds register information:

  Base   : 0x00003082 (decimal 12418)
  Limit  : 472

Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> PA or violation?
  VA  1: 0x0109 (decimal:265) -> PA or violation?
  VA  2: 0x020b (decimal:523) -> PA or violation?
  VA  3: 0x019e (decimal:414) -> PA or violation?
  VA  4: 0x0322 (decimal:802) -> PA or violation?
```

For each virtual address, either write down the physical address it 
translates to OR write down that it is an out-of-bounds address 
(a segmentation violation). For this problem, you should assume a 
simple virtual address space of a given size.
对于每个虚拟地址，要么写下它转换后的物理地址，要么写下它是一个
越界地址（段错误）。对于这个问题，你应该假设一个给定大小的简单
虚拟地址空间。

As you can see, the homework simply generates randomized virtual
addresses. For each, you should determine whether it is in bounds, and if so,
determine to which physical address it translates. Running with -c (the
"compute this for me" flag) gives us the results of these translations, i.e.,
whether they are valid or not, and if valid, the resulting physical
addresses. For convenience, all numbers are given both in hex and decimal.
如你所见，本作业只是生成随机的虚拟地址。对于每个地址，你应该确
定它是否在界限内，如果是，则确定它转换到哪个物理地址。使用 -c 
标志（"帮我计算"标志）运行程序会给出这些转换的结果，即它们是否
有效，如果有效，则给出结果物理地址。为方便起见，所有数字都以十六
进制和十进制两种形式给出。

```sh
prompt> ./relocation.py -c
...
Virtual Address Trace
  VA  0: 0x01ae (decimal:430) -> VALID: 0x00003230 (dec:12848)
  VA  1: 0x0109 (decimal:265) -> VALID: 0x0000318b (dec:12683)
  VA  2: 0x020b (decimal:523) -> SEGMENTATION VIOLATION
  VA  3: 0x019e (decimal:414) -> VALID: 0x00003220 (dec:12832)
  VA  4: 0x0322 (decimal:802) -> SEGMENTATION VIOLATION
```

With a base address of 12418 (decimal), address 430 is within bounds (i.e., it
is less than the limit register of 472) and thus translates to 430 added to
12418 or 12848. A few of the addresses shown above are out of bounds (523,
828), as they are in excess of the bounds. Pretty simple, no? Indeed, that is
one of the beauties of base and bounds: it's so darn simple!
基址为 12418（十进制）时，地址 430 在界限内（即小于界限寄存器
的值 472），因此转换为 430 加上 12418，即 12848。上面显示的一
些地址是越界的（523、802），因为它们超过了界限。相当简单，不是
吗？确实，这就是基址-界限的优点之一：它非常简单！

There are a few flags you can use to control what's going on better:
你可以使用几个标志来更好地控制程序的行为：

```sh
prompt> ./relocation.py -h
Usage: relocation.py [options]

Options:
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -a ASIZE, --asize=ASIZE address space size (e.g., 16, 64k, 32m)
  -p PSIZE, --physmem=PSIZE physical memory size (e.g., 16, 64k)
  -n NUM, --addresses=NUM # of virtual addresses to generate
  -b BASE, --b=BASE     value of base register
  -l LIMIT, --l=LIMIT   value of limit register
  -c, --compute         compute answers for me
```

In particular, you can control the virtual address-space size (-a), the size
of physical memory (-p), the number of virtual addresses to generate (-n), and
the values of the base and bounds registers for this process (-b and -l,
respectively).
特别是，你可以控制虚拟地址空间大小（-a）、物理内存大小（-p）、要
生成的虚拟地址数量（-n），以及此进程的基址和界限寄存器的值（分
别为 -b 和 -l）。

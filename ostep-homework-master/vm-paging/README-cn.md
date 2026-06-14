
# Overview

In this homework, you will use a simple program, which is known as
paging-linear-translate.py, to see if you understand how simple
virtual-to-physical address translation works with linear page tables. To run
the program, remember to either type just the name of the program
(./paging-linear-translate.py) or possibly this (python
paging-linear-translate.py). When you run it with the -h (help) flag, you 
see:

> # 概述
> 
> 在本次作业中，你将使用一个简单的程序
> `paging-linear-translate.py`，看看你是否理解了线性页表如何进行简单的
> 虚拟地址到物理地址的翻译。运行程序时，记得要么直接输入程序名
> （`./paging-linear-translate.py`），要么使用
> `python paging-linear-translate.py`。当你用 `-h`（帮助）参数运行时，
> 会看到：

```sh
prompt> ./paging-linear-translate.py -h
Usage: paging-linear-translate.py [options]

Options:
-h, --help              show this help message and exit
-s SEED, --seed=SEED    the random seed
-a ASIZE, --asize=ASIZE 
                        address space size (e.g., 16, 64k, ...)
-p PSIZE, --physmem=PSIZE
                        physical memory size (e.g., 16, 64k, ...)
-P PAGESIZE, --pagesize=PAGESIZE
                        page size (e.g., 4k, 8k, ...)
-n NUM, --addresses=NUM number of virtual addresses to generate
-u USED, --used=USED    percent of address space that is used
-v                      verbose mode
-c                      compute answers for me
```

First, run the program without any arguments:

> 首先，不带任何参数运行程序：

```sh
prompt> ./paging-linear-translate.py 
ARG seed 0
ARG address space size 16k
ARG phys mem size 64k
ARG page size 4k
ARG verbose False

The format of the page table is simple:
The high-order (left-most) bit is the VALID bit.
  If the bit is 1, the rest of the entry is the PFN.
  If the bit is 0, the page is not valid.
Use verbose mode (-v) if you want to print the VPN # by
each entry of the page table.

Page Table (from entry 0 down to the max size)
   0x8000000c
   0x00000000
   0x00000000
   0x80000006

Virtual Address Trace
  VA  0: 0x00003229 (decimal:    12841) --> PA or invalid?
  VA  1: 0x00001369 (decimal:     4969) --> PA or invalid?
  VA  2: 0x00001e80 (decimal:     7808) --> PA or invalid?
  VA  3: 0x00002556 (decimal:     9558) --> PA or invalid?
  VA  4: 0x00003a1e (decimal:    14878) --> PA or invalid?
```

For each virtual address, write down the physical address it 
translates to OR write down that it is an out-of-bounds 
address (e.g., a segmentation fault).

> 对于每个虚拟地址，请写出它翻译后的物理地址，或者写出它是一个越界地址
> （例如，段错误/segmentation fault）。

As you can see, what the program provides for you is a page table for a
particular process (remember, in a real system with linear page tables, there
is one page table per process; here we just focus on one process, its address
space, and thus a single page table). The page table tells you, for each
virtual page number (VPN) of the address space, that the virtual page is
mapped to a particular physical frame number (PFN) and thus valid, or not
valid.

> 如你所见，程序提供给你的是某个特定进程的页表（记住，在真实的线性页表
> 系统中，每个进程都有一个页表；这里我们只关注一个进程、它的地址空间，
> 因此只有一个页表）。页表告诉你，对于地址空间中的每个虚拟页号（VPN），
> 该虚拟页是映射到了某个物理帧号（PFN）从而有效，还是无效。

The format of the page-table entry is simple: the left-most (high-order) bit
is the valid bit; the remaining bits, if valid is 1, is the PFN. 

> 页表项的格式很简单：最左边（最高位）bit 是有效位；如果有效位为 1，
> 其余位就是 PFN。

In the example above, the page table maps VPN 0 to PFN 0xc (decimal 12), VPN 3
to PFN 0x6 (decimal 6), and leaves the other two virtual pages, 1 and 2, as
not valid. 

> 在上面的例子中，页表将 VPN 0 映射到 PFN 0xc（十进制 12），VPN 3 映射
> 到 PFN 0x6（十进制 6），其余两个虚拟页（VPN 1 和 VPN 2）是无效的。

Because the page table is a linear array, what is printed above is a replica
of what you would see in memory if you looked at the bits yourself. However,
it is sometimes easier to use this simulator if you run with the verbose flag
(-v); this flag also prints out the VPN (index) into the page table. From the
example above, run with the -v flag:

> 由于页表是一个线性数组，上面打印的内容就是你在内存中直接查看 bit 时
> 所见内容的复现。不过，使用 `-v`（详细模式）运行模拟器有时会更容易理解；
> 这个参数还会打印出 VPN（在页表中的索引）。以上面的例子运行 `-v` 参数：

```sh
Page Table (from entry 0 down to the max size)
  [       0]   0x8000000c
  [       1]   0x00000000
  [       2]   0x00000000
  [       3]   0x80000006
```

Your job, then, is to use this page table to translate the virtual addresses
given to you in the trace to physical addresses. Let's look at the first one:
VA 0x3229. To translate this virtual address into a physical address, we first
have to break it up into its constituent components: a virtual page number and
an offset. We do this by noting down the size of the address space and the
page size. In this example, the address space is set to 16KB (a very small
address space) and the page size is 4KB. Thus, we know that there are 14 bits
in the virtual address, and that the offset is 12 bits, leaving 2 bits for the
VPN. Thus, with our address 0x3229, which is binary 11 0010 0010 1001, we know
the top two bits specify the VPN. Thus, 0x3229 is on virtual page 3 with an
offset of 0x229.

> 你的任务就是使用这个页表，将 trace 中给出的虚拟地址翻译为物理地址。
> 我们来看第一个：VA 0x3229。要将这个虚拟地址翻译为物理地址，我们首先
> 需要将它分解为两个组成部分：虚拟页号和页内偏移。我们通过记录地址空间
> 大小和页面大小来完成这一步。在这个例子中，地址空间设为 16KB
> （一个非常小的地址空间），页面大小为 4KB。因此，我们知道虚拟地址共有
> 14 位，页内偏移占 12 位，VPN 占剩下的 2 位。因此，对于地址 0x3229，
> 其二进制为 11 0010 0010 1001，我们知道高两位指定了 VPN。所以，0x3229
> 位于虚拟页 3，偏移量为 0x229。

We next look in the page table to see if VPN 3 is valid and mapped to some
physical frame or invalid, and we see that it is indeed valid (the high bit is
1) and mapped to physical page 6. Thus, we can form our final physical address
by taking the physical page 6 and adding it onto the offset, as follows:
0x6000 (the physical page, shifted into the proper spot) OR 0x0229 (the
offset), yielding the final physical address: 0x6229. Thus, we can see that
virtual address 0x3229 translates to physical address 0x6229 in this example.

> 接下来我们查看页表，看 VPN 3 是有效并映射到了某个物理帧，还是无效。
> 我们看到它确实是有效的（高位为 1），且映射到了物理页 6。因此，我们可以
> 这样构造最终的物理地址：取物理页 6，将其拼上偏移量，如下所示：0x6000
> （物理页，移位到正确位置）OR 0x0229（偏移量），得到最终的物理地址：
> 0x6229。因此，在这个例子中，虚拟地址 0x3229 翻译为物理地址 0x6229。

To see the rest of the solutions (after you have computed them yourself!),
just run with the -c flag (as always):

> 要查看其余答案（请在自己计算之后再查看！），只需使用 `-c` 参数运行
> （一如既往）：

```sh
...
VA  0: 00003229 (decimal: 12841) --> 00006229 (25129) [VPN 3]
VA  1: 00001369 (decimal:  4969) --> Invalid (VPN 1 not valid)
VA  2: 00001e80 (decimal:  7808) --> Invalid (VPN 1 not valid)
VA  3: 00002556 (decimal:  9558) --> Invalid (VPN 2 not valid)
VA  4: 00003a1e (decimal: 14878) --> 00006a1e (27166) [VPN 3]
```

Of course, you can change many of these parameters to make more interesting
problems. Run the program with the -h flag to see what options there are:

> 当然，你可以改变这些参数来生成更有趣的问题。使用 `-h` 参数运行程序
> 查看有哪些可用选项：

* The -s flag changes the random seed and thus generates different page table values as well as different virtual addresses to translate.
* The -a flag changes the size of the address space.
* The -p flag changes the size of physical memory.
* The -P flag changes the size of a page.
* The -n flag can be used to generate more addresses to translate (instead of the default 5).
* The -u flag changes the fraction of mappings that are valid, from 0% (-u 0) up to 100% (-u 100). The default is 50, which means that roughly 1/2 of the pages in the virtual address space will be valid.
* The -v flag prints out the VPN numbers to make your life easier.

> * **`-s`** 改变随机种子，从而生成不同的页表值以及不同的待翻译虚拟地址。
> * **`-a`** 改变地址空间的大小。
> * **`-p`** 改变物理内存的大小。
> * **`-P`** 改变页面的大小。
> * **`-n`** 用于生成更多待翻译的地址（默认是 5 个）。
> * **`-u`** 改变有效映射的比例，范围从 0%（`-u 0`）到 100%（`-u 100`）。
>   默认值是 50，意味着虚拟地址空间中大约 1/2 的页面是有效的。
> * **`-v`** 打印出 VPN 编号，让你做题更轻松。





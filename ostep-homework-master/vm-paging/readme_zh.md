# Overview
# 概述

In this homework, you will use a simple program, which is known as
paging-linear-translate.py, to see if you understand how simple
virtual-to-physical address translation works with linear page tables. To run
the program, remember to either type just the name of the program
(./paging-linear-translate.py) or possibly this (python
paging-linear-translate.py). When you run it with the -h (help) flag, you 
see:
在本次作业中，你将使用一个名为 paging-linear-translate.py 的简单程序，来检验你是否理
解使用线性页表进行虚拟地址到物理地址转换的工作原理。运行程序时，请记住输入程序名称
（./paging-linear-translate.py）或使用 python 运行（python paging-linear-translate.py）。
使用 -h（帮助）标志运行时，你会看到：

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
首先，不带任何参数运行程序：

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
对于每个虚拟地址，写下它转换后的物理地址，或者注明它是一个无效地址（例如段错误）。

As you can see, what the program provides for you is a page table for a
particular process (remember, in a real system with linear page tables, there
is one page table per process; here we just focus on one process, its address
space, and thus a single page table). The page table tells you, for each
virtual page number (VPN) of the address space, that the virtual page is
mapped to a particular physical frame number (PFN) and thus valid, or not
valid.
如你所见，程序为你提供了一个特定进程的页表（请记住，在使用线性页表的真实系统中，每个进程
都有一个页表；这里我们只关注一个进程、它的地址空间，因此只有一个页表）。页表告诉你，对于
地址空间的每个虚拟页号（VPN），该虚拟页是否映射到某个物理帧号（PFN），即是否有效。

The format of the page-table entry is simple: the left-most (high-order) bit
is the valid bit; the remaining bits, if valid is 1, is the PFN. 
页表项的格式很简单：最左边（最高位）的位是有效位；如果有效位为 1，则其余位是物理帧号（PFN）。

In the example above, the page table maps VPN 0 to PFN 0xc (decimal 12), VPN 3
to PFN 0x6 (decimal 6), and leaves the other two virtual pages, 1 and 2, as
not valid. 
在上面的例子中，页表将 VPN 0 映射到 PFN 0xc（十进制 12），VPN 3 映射到 PFN 0x6
（十进制 6），而其他两个虚拟页（1 和 2）则标记为无效。

Because the page table is a linear array, what is printed above is a replica
of what you would see in memory if you looked at the bits yourself. However,
it is sometimes easier to use this simulator if you run with the verbose flag
(-v); this flag also prints out the VPN (index) into the page table. From the
example above, run with the -v flag:
由于页表是一个线性数组，上面打印的内容就是你自己查看内存中的位时会看到的内容的副本。
然而，有时使用详细模式（-v）运行这个模拟器会更容易；这个标志还会打印出页表中的 VPN（索引）。
从上面的例子来看，使用 -v 标志运行：

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
那么，你的任务就是使用这个页表将跟踪中给出的虚拟地址转换为物理地址。
让我们来看第一个：VA 0x3229。要将这个虚拟地址转换为物理地址，我们首先必须将其分解为
组成部分：虚拟页号和偏移量。我们通过记录地址空间大小和页大小来做到这一点。在这个例子中，
地址空间设置为 16KB（非常小的地址空间），页大小为 4KB。因此，我们知道虚拟地址有 14 位，
偏移量是 12 位，剩下 2 位用于 VPN。因此，对于地址 0x3229（二进制 11 0010 0010 1001），
我们知道最高两位指定 VPN。因此，0x3229 在虚拟页 3 上，偏移量为 0x229。

We next look in the page table to see if VPN 3 is valid and mapped to some
physical frame or invalid, and we see that it is indeed valid (the high bit is
1) and mapped to physical page 6. Thus, we can form our final physical address
by taking the physical page 6 and adding it onto the offset, as follows:
0x6000 (the physical page, shifted into the proper spot) OR 0x0229 (the
offset), yielding the final physical address: 0x6229. Thus, we can see that
virtual address 0x3229 translates to physical address 0x6229 in this example.
接下来我们查看页表，看看 VPN 3 是否有效并映射到某个物理帧，我们看到它确实有效（高位为 1）
并映射到物理页 6。因此，我们可以通过将物理页 6 加上偏移量来形成最终的物理地址，
如下所示：0x6000（物理页，移到正确的位置）OR 0x0229（偏移量），得到最终的物理地址：0x6229。
因此，我们可以看到在这个例子中，虚拟地址 0x3229 转换为物理地址 0x6229。

To see the rest of the solutions (after you have computed them yourself!),
just run with the -c flag (as always):
要查看其余的答案（在你自己计算之后！），只需使用 -c 标志运行（一如既往）：

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
当然，你可以更改其中许多参数来创建更有趣的问题。使用 -h 标志运行程序查看可用选项：

* The -s flag changes the random seed and thus generates different page table values as well as different virtual addresses to translate.
* -s 标志更改随机种子，从而生成不同的页表值以及不同的虚拟地址进行转换。
* The -a flag changes the size of the address space.
* -a 标志更改地址空间的大小。
* The -p flag changes the size of physical memory.
* -p 标志更改物理内存的大小。
* The -P flag changes the size of a page.
* -P 标志更改页的大小。
* The -n flag can be used to generate more addresses to translate (instead of the default 5).
* -n 标志可用于生成更多要转换的地址（而不是默认的 5 个）。
* The -u flag changes the fraction of mappings that are valid, from 0% (-u 0) up to 100% (-u 100). The default is 50, which means that roughly 1/2 of the pages in the virtual address space will be valid.
* -u 标志更改有效映射的比例，从 0%（-u 0）到 100%（-u 100）。默认为 50，这意味着虚拟地址空间中大约 1/2 的页将是有效的。
* The -v flag prints out the VPN numbers to make your life easier.
* -v 标志打印出 VPN 编号，让你的工作更轻松。

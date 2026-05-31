# Overview

This program, malloc.py, allows you to see how a simple
memory allocator works.
> 这个程序 malloc.py 可以让你了解简单的内存分配器
> 是如何工作的。

Here are the options that you have at your disposal:
> 以下是你可以使用的选项：

```sh
  -h, --help            show this help message and exit
  -s SEED, --seed=SEED  the random seed
  -S HEAPSIZE, --size=HEAPSIZE
                        size of the heap
  -b BASEADDR, --baseAddr=BASEADDR
                        base address of heap
  -H HEADERSIZE, --headerSize=HEADERSIZE
                        size of the header
  -a ALIGNMENT, --alignment=ALIGNMENT
                        align allocated units to size; -1->no align
  -p POLICY, --policy=POLICY
                        list search (BEST, WORST, FIRST)
  -l ORDER, --listOrder=ORDER
                        list order (ADDRSORT, SIZESORT+, SIZESORT-
                        INSERT-FRONT, INSERT-BACK)
  -C, --coalesce        coalesce the free list?
  -n OPSNUM, --numOps=OPSNUM
                        number of random ops to generate
  -r OPSRANGE, --range=OPSRANGE
                        max alloc size
  -P OPSPALLOC, --percentAlloc=OPSPALLOC
                        percent of ops that are allocs
  -A OPSLIST, --allocList=OPSLIST
                        instead of random, list of ops (+10,-0,etc)
  -c, --compute         compute answers for me
```
> ```
>   -h, --help            显示帮助信息并退出
>   -s SEED, --seed=SEED  随机种子
>   -S HEAPSIZE, --size=HEAPSIZE
>                         堆大小
>   -b BASEADDR, --baseAddr=BASEADDR
>                         堆的基地址
>   -H HEADERSIZE, --headerSize=HEADERSIZE
>                         头部大小
>   -a ALIGNMENT, --alignment=ALIGNMENT
>                         分配单元对齐大小；-1表示不对齐
>   -p POLICY, --policy=POLICY
>                         列表搜索策略（BEST, WORST, FIRST）
>   -l ORDER, --listOrder=ORDER
>                         列表顺序（ADDRSORT, SIZESORT+, SIZESORT-
>                         INSERT-FRONT, INSERT-BACK）
>   -C, --coalesce        是否合并空闲列表？
>   -n OPSNUM, --numOps=OPSNUM
>                         生成的随机操作数
>   -r OPSRANGE, --range=OPSRANGE
>                         最大分配大小
>   -P OPSPALLOC, --percentAlloc=OPSPALLOC
>                         分配操作的百分比
>   -A OPSLIST, --allocList=OPSLIST
>                         指定操作列表（如+10,-0等）而非随机
>   -c, --compute         帮我计算答案
> ```

One way to use it is to have the program generate some
random allocation/free operations and for you to see if
you can figure out what the free list would look like.
> 使用它的一种方式是让程序生成一些随机的分配/
> 释放操作，看看你能否推断出空闲列表的样子。

As well as the success or failure of each operation.
> 以及每个操作的成功或失败情况。

Here is a simple example:
> 以下是一个简单示例：

```sh
prompt> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT \
                    -p BEST -n 5 

ptr[0] = Alloc(3)  returned ?
List?

Free(ptr[0]) returned ?
List?

ptr[1] = Alloc(5)  returned ?
List?

Free(ptr[1]) returned ?
List?

ptr[2] = Alloc(8)  returned ?
List?
```
> ```
> 命令行> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT \
>                     -p BEST -n 5 
> 
> ptr[0] = Alloc(3)  返回值？
> 列表状态？
> 
> Free(ptr[0]) 返回值？
> 列表状态？
> 
> ptr[1] = Alloc(5)  返回值？
> 列表状态？
> 
> Free(ptr[1]) 返回值？
> 列表状态？
> 
> ptr[2] = Alloc(8)  返回值？
> 列表状态？
> ```

In this example, we specify a heap of size 100 bytes
(-S 100), starting at address 1000 (-b 1000).
> 在这个示例中，我们指定了一个大小为 100 字节的堆
> （-S 100），起始地址为 1000（-b 1000）。

We specify an additional 4 bytes of header per allocated
block (-H 4), and make sure each allocated space rounds
up to the nearest 4-byte free chunk in size (-a 4).
> 我们指定每个分配块有 4 字节的头部（-H 4），并确保
> 每个分配空间向上舍入到最接近的 4 字节空闲块
> 大小（-a 4）。

We specify that the free list be kept ordered by
address (increasing).
> 我们指定空闲列表按地址（递增）排序。

Finally, we specify a "best fit" free-list searching
policy (-p BEST), and ask for 5 random operations
to be generated (-n 5).
> 最后，我们指定"最佳适配"空闲列表搜索策略
> （-p BEST），并要求生成 5 个随机操作（-n 5）。

The results of running this are above; your job is
to figure out what each allocation/free operation
returns, as well as the state of the free list after
each operation.
> 运行结果如上所示；你的任务是推断每个分配/
> 释放操作返回什么，以及每次操作后空闲列表
> 的状态。

Here we look at the results by using the -c option.
> 这里我们使用 -c 选项查看结果。

```sh
prompt> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT \
                    -p BEST -n 5 -c

ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]

Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:92 ]

ptr[1] = Alloc(5)  returned 1012 (searched 2 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1020 sz:80 ]

Free(ptr[1]) returned 0
Free List [ Size 3 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:12 ]
                       [ addr:1020 sz:80 ]

ptr[2] = Alloc(8)  returned 1012 (searched 3 elements)
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1020 sz:80 ]
```
> ```
> 命令行> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT \
>                     -p BEST -n 5 -c
> 
> ptr[0] = Alloc(3)  返回 1004（搜索了 1 个元素）
> 空闲列表 [ 大小 1 ]:  [ 地址:1008 大小:92 ]
> 
> Free(ptr[0]) 返回 0
> 空闲列表 [ 大小 2 ]:  [ 地址:1000 大小:8 ] [ 地址:1008 大小:92 ]
> 
> ptr[1] = Alloc(5)  返回 1012（搜索了 2 个元素）
> 空闲列表 [ 大小 2 ]:  [ 地址:1000 大小:8 ] [ 地址:1020 大小:80 ]
> 
> Free(ptr[1]) 返回 0
> 空闲列表 [ 大小 3 ]:  [ 地址:1000 大小:8 ] [ 地址:1008 大小:12 ]
>                      [ 地址:1020 大小:80 ]
> 
> ptr[2] = Alloc(8)  返回 1012（搜索了 3 个元素）
> 空闲列表 [ 大小 2 ]:  [ 地址:1000 大小:8 ] [ 地址:1020 大小:80 ]
> ```

As you can see, the first allocation operation returns
the following information:
> 如你所见，第一个分配操作返回以下信息：

ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]
> ptr[0] = Alloc(3)  返回 1004（搜索了 1 个元素）
> 空闲列表 [ 大小 1 ]:  [ 地址:1008 大小:92 ]

Because the initial state of the free list is just one
large element, it is easy to guess that the Alloc(3)
request will succeed.
> 因为空闲列表的初始状态只有一个大元素，很容易
> 推断出 Alloc(3) 请求会成功。

Further, it will just return the first chunk of memory
and make the remainder into a free list.
> 此外，它只会返回第一块内存，并将剩余部分
> 放入空闲列表。

The pointer returned will be just beyond the header
(address:1004), and the allocated space is rounded
up to 4 bytes, leaving the free list with 92 bytes
starting at 1008.
> 返回的指针将正好在头部之后（地址：1004），
> 分配的空间向上舍入到 4 字节，空闲列表从
> 1008 开始还剩 92 字节。

The next operation is a Free, of "ptr[0]" which is
what stores the results of the previous allocation
request.
> 下一个操作是释放 "ptr[0]"，它存储了上一次
> 分配请求的结果。

As you can expect, this free will succeed (thus
returning "0"), and the free list now looks a little
more complicated:
> 如你所料，这次释放会成功（因此返回"0"），
> 空闲列表现在看起来有点复杂：

```sh
Free(ptr[0]) returned 0
Free List [ Size 2 ]:  [ addr:1000 sz:8 ] [ addr:1008 sz:92 ]
```
> ```
> Free(ptr[0]) 返回 0
> 空闲列表 [ 大小 2 ]:  [ 地址:1000 大小:8 ] [ 地址:1008 大小:92 ]
> ```

Indeed, because we are NOT coalescing the free list,
we now have two elements on it, the first being 8
bytes large and holding the just-returned space, and
the second being the 92-byte chunk.
> 确实，因为我们没有合并空闲列表，现在列表上
> 有两个元素，第一个是 8 字节，包含刚释放的
> 空间，第二个是 92 字节的块。

We can indeed turn on coalescing via the -C flag,
and the result is:
> 我们确实可以通过 -C 标志开启合并，结果如下：

```sh
prompt> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT \
                    -p BEST -n 5 -c -C
ptr[0] = Alloc(3)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1008 sz:92 ]

Free(ptr[0]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ]

ptr[1] = Alloc(5)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1012 sz:88 ]

Free(ptr[1]) returned 0
Free List [ Size 1 ]:  [ addr:1000 sz:100 ]

ptr[2] = Alloc(8)  returned 1004 (searched 1 elements)
Free List [ Size 1 ]:  [ addr:1012 sz:88 ]
```
> ```
> 命令行> ./malloc.py -S 100 -b 1000 -H 4 -a 4 -l ADDRSORT \
>                     -p BEST -n 5 -c -C
> ptr[0] = Alloc(3)  返回 1004（搜索了 1 个元素）
> 空闲列表 [ 大小 1 ]:  [ 地址:1008 大小:92 ]
> 
> Free(ptr[0]) 返回 0
> 空闲列表 [ 大小 1 ]:  [ 地址:1000 大小:100 ]
> 
> ptr[1] = Alloc(5)  返回 1004（搜索了 1 个元素）
> 空闲列表 [ 大小 1 ]:  [ 地址:1012 大小:88 ]
> 
> Free(ptr[1]) 返回 0
> 空闲列表 [ 大小 1 ]:  [ 地址:1000 大小:100 ]
> 
> ptr[2] = Alloc(8)  返回 1004（搜索了 1 个元素）
> 空闲列表 [ 大小 1 ]:  [ 地址:1012 大小:88 ]
> ```

You can see that when the Free operations take place,
the free list is coalesced as expected.
> 你可以看到，当释放操作发生时，空闲列表按预期
> 进行了合并。

There are some other interesting options to explore:
> 还有一些其他有趣的选项可以探索：

* `-p BEST` or `-p WORST` or `-p FIRST`: This option
  lets you use these three different strategies to
  look for a chunk of memory to use during an
  allocation request
> * `-p BEST` 或 `-p WORST` 或 `-p FIRST`：此选项允许你
>   使用这三种不同的策略在分配请求期间查找内存块

* `-l ADDRSORT` or `-l SIZESORT+` or `-l SIZESORT-`
  or `-l INSERT-FRONT` or `-l INSERT-BACK`: This
  option lets you keep the free list in a particular
  order.
> * `-l ADDRSORT` 或 `-l SIZESORT+` 或 `-l SIZESORT-`
>   或 `-l INSERT-FRONT` 或 `-l INSERT-BACK`：此选项允许
>   你以特定顺序保持空闲列表。

Say sorted by address of the free chunk, size of
free chunk (either increasing with a + or decreasing
with a -), or simply returning free chunks to the
front (INSERT-FRONT) or back (INSERT-BACK) of the
free list.
> 例如按空闲块的地址排序、按空闲块的大小排序
> （+ 表示递增，- 表示递减），或者简单地将
> 空闲块返回到空闲列表的前面（INSERT-FRONT）
> 或后面（INSERT-BACK）。

* `-A list_of_ops`: This option lets you specify an
  exact series of requests instead of randomly-
  generated ones.
> * `-A list_of_ops`：此选项允许你指定一系列
>   确切的请求，而不是随机生成的请求。

For example, running with the flag "-A +10,+10,+10,-0,-2"
will allocate three chunks of size 10 bytes (plus header),
and then free the first one ("-0") and then free the
third one ("-2").
> 例如，使用标志 "-A +10,+10,+10,-0,-2" 运行将分配
> 三个大小为 10 字节的块（加上头部），然后释放第一个
> （"-0"），然后释放第三个（"-2"）。

What will the free list look like then?
> 那么空闲列表会是什么样子呢？

Those are the basics. Use the questions from the
book chapter to explore more, or create new and
interesting questions yourself to better understand
how allocators function.
> 以上是基础知识。使用本书章节中的问题进行更多探索，
> 或者自己创建新的有趣问题，以更好地理解分配器的
> 工作原理。

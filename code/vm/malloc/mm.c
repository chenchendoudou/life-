/* 
 * Simple, 32-bit and 64-bit clean allocator based on implicit free
 * lists, first fit placement, and boundary tag coalescing, as described
 * in the CS:APP2e text. Blocks must be aligned to doubleword (8 byte) 
 * boundaries. Minimum block size is 16 bytes. 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"

/*
 * If NEXT_FIT defined use next fit search, else use first fit search 
 */
#define NEXT_FITx

/* $begin mallocmacros */
/* Basic constants and macros */

#define WSIZE       4       /* Word and header/footer size (bytes) */ //line:vm:mm:beginconst
#define DSIZE       8       /* Doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */  //line:vm:mm:endconst 

#define MAX(x, y) ((x) > (y)? (x) : (y))  

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc)) //line:vm:mm:pack

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))            //line:vm:mm:get
#define PUT(p, val)  (*(unsigned int *)(p) = (val))    //line:vm:mm:put

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)                   //line:vm:mm:getsize
#define GET_ALLOC(p) (GET(p) & 0x1)                    //line:vm:mm:getalloc

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)                      //line:vm:mm:hdrp
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) //line:vm:mm:ftrp

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) //line:vm:mm:nextblkp
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) //line:vm:mm:prevblkp
/* $end mallocmacros */

/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */  
#ifdef NEXT_FIT
static char *rover;           /* Next fit rover */
#endif

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void checkheap(int verbose);
static void checkblock(void *bp);

/* 
 * mm_init - Initialize the memory manager 
 */
/* $begin mminit */
int mm_init(void) 
{
    /* Create the initial empty heap */
    //申请 16 字节空间，写入 4 个 Word                    
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1) //line:vm:mm:begininit
	return -1;
       //对齐填充，保证 8 字节对齐 
    PUT(heap_listp, 0);                          /* Alignment padding */
    //序言页眉，固定 8 字节已分配块
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */ 
    //序言页脚，与页眉配对
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */ 
    //尾声页眉，标记堆末尾
    PUT(heap_listp + (3*WSIZE), PACK(0, 1));     /* Epilogue header */
    //移动堆指针到序言页脚之后
    heap_listp += (2*WSIZE);                     //line:vm:mm:endinit  
/* $end mminit */

#ifdef NEXT_FIT
    rover = heap_listp;
#endif
/* $begin mminit */

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    //扩展堆，申请 CHUNKSIZE 字节空间
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
	return -1;
    return 0;
}
/* $end mminit */

/* 
 * mm_malloc - Allocate a block with at least size bytes of payload 
 */
/* $begin mmmalloc */
/*
mm_malloc(size)
    │
    ├─ ① 堆未初始化？→ mm_init()
    ├─ ② size == 0？→ 返回 NULL
    ├─ ③ 计算对齐后的块大小 asize
    │
    ├─ ④ 在空闲链表中查找合适的块
    │     ├─ 找到 → place 放置 → 返回 bp
    │     └─ 未找到 ↓
    │
    └─ ⑤ 扩展堆（max(asize, CHUNKSIZE)）
          └─ extend_heap → place 放置 → 返回 bp
*/
void *mm_malloc(size_t size) 
{
    //调整块大小，包括头和尾的开销
    size_t asize;      /* Adjusted block size */
    //扩展堆的大小，确保有足够空间分配块
    //如果当前堆大小不足，扩展堆大小为 CHUNKSIZE 字节
    size_t extendsize; /* Amount to extend heap if no fit */
    //块指针，指向分配的内存块
    char *bp;      

/* $end mmmalloc */
    //
    if (heap_listp == 0){
	mm_init();
    }
/* $begin mmmalloc */
    /* Ignore spurious requests */
    if (size == 0)
	return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    // 用户请求 + 页眉 + 页脚 + 对齐填充。                
    if (size <= DSIZE)                                          //line:vm:mm:sizeadjust1
	asize = 2*DSIZE;                                        //line:vm:mm:sizeadjust2
    else
	asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE); //line:vm:mm:sizeadjust3

    /* Search the free list for a fit */
    //查找空闲块
    if ((bp = find_fit(asize)) != NULL) {  //line:vm:mm:findfitcall
	//place 函数会将找到的空闲块分割：若剩余空间 ≥ 16 字节，则分为已分配块 + 新的空闲块；
    //否则整个块都分配出去（避免产生太小的残余碎片）
        place(bp, asize);                  //line:vm:mm:findfitplace
	return bp;
    }

    /* No fit found. Get more memory and place the block */
    //CHUNKSIZE = 4096 字节，是堆的扩展粒度
    //若用户请求 asize > 4096，则按实际需要扩展
    //若用户请求 asize < 4096，仍扩展 4096 字节，避免频繁的系统调用
    extendsize = MAX(asize,CHUNKSIZE);                 //line:vm:mm:growheap1
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)  
	return NULL;                                  //line:vm:mm:growheap2
    place(bp, asize);                                 //line:vm:mm:growheap3
    return bp;
} 
/* $end mmmalloc */

/* 
 * mm_free - Free a block 
 */
/* $begin mmfree */
void mm_free(void *bp)
{
/* $end mmfree */
    if(bp == 0) 
	return;

/* $begin mmfree */
    size_t size = GET_SIZE(HDRP(bp));
/* $end mmfree */
    if (heap_listp == 0){
	mm_init();
    }
/* $begin mmfree */

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    coalesce(bp);
}

/* $end mmfree */
/*
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block
 */
/* $begin mmfree */
//边界标签合并：在释放块时检查前后相邻块状态，将连续空闲块合并成更大的空闲块，消除外部碎片。
static void *coalesce(void *bp) 
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {            /* Case 1 */
	return bp;
    }

    else if (prev_alloc && !next_alloc) {      /* Case 2 */
	size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size,0));
    }

    else if (!prev_alloc && next_alloc) {      /* Case 3 */
	size += GET_SIZE(HDRP(PREV_BLKP(bp)));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
	bp = PREV_BLKP(bp);
    }

    else {                                     /* Case 4 */
	size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
	    GET_SIZE(FTRP(NEXT_BLKP(bp)));
	PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
	PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
	bp = PREV_BLKP(bp);
    }
/* $end mmfree */
#ifdef NEXT_FIT
    /* Make sure the rover isn't pointing into the free block */
    /* that we just coalesced */
    if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp))) 
	rover = bp;
#endif
/* $begin mmfree */
    return bp;
}
/* $end mmfree */

/*
 * mm_realloc - Naive implementation of realloc
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t oldsize;
    void *newptr;

    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0) {
	mm_free(ptr);
	return 0;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if(ptr == NULL) {
	return mm_malloc(size);
    }

    newptr = mm_malloc(size);

    /* If realloc() fails the original block is left untouched  */
    if(!newptr) {
	return 0;
    }

    /* Copy the old data. */
    oldsize = GET_SIZE(HDRP(ptr));
    if(size < oldsize) oldsize = size;
    memcpy(newptr, ptr, oldsize);

    /* Free the old block. */
    mm_free(ptr);

    return newptr;
}

/* 
 * checkheap - We don't check anything right now. 
 */
void mm_checkheap(int verbose)  
{ 
}

/* 
 * The remaining routines are internal helper routines 
 */

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 *
 * extend_heap(words)
    │
    ├─ ① 对齐计算：size = 偶数个 word → 保证 8 字节对齐
    │
    ├─ ② 申请内存：mem_sbrk(size)
    │     失败 → 返回 NULL
    │     成功 → bp 指向新空间起始
    │
    ├─ ③ 初始化新空闲块的页眉和页脚
    │
    ├─ ④ 放置新的尾声页眉（覆盖旧的）
    │
    └─ ⑤ coalesce(bp)：与前一个空闲块合并，返回合并后的指针
 * /
/* $begin mmextendheap */
//扩展堆：申请内存，初始化新空闲块，合并与前一个空闲块  
//参数：words - 要扩展的字节数，单位为 word 数
//返回值：bp - 指向新空间起始的指针，若失败则返回 NULL
//说明：若申请内存失败，返回 NULL；否则，返回新空间起始地址。
static void *extend_heap(size_t words) 
{
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    //WSIZE = 4，两个 word = 8 字节 = 一个 DSIZE。
    //强制偶数个 word 等价于强制申请的字节数是 8 的倍数。
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE; //line:vm:mm:beginextend
    if ((long)(bp = mem_sbrk(size)) == -1)  
	return NULL;                                        //line:vm:mm:endextend

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */   //line:vm:mm:freeblockhdr
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */   //line:vm:mm:freeblockftr
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */ //line:vm:mm:newepihdr

    /* Coalesce if the previous block was free */
    return coalesce(bp);                                          //line:vm:mm:returnblock
}
/* $end mmextendheap */

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
/* $begin mmplace */
/* $begin mmplace-proto */            
//参数：bp - 指向空闲块的指针
//参数：asize - 请求的内存大小，单位为字节
//返回值：无
//说明：将请求的内存大小 asize 放置在空闲块 bp 的起始位置，
//若空闲块 bp 的大小大于 asize，将 bp 分成两个空闲块，
//一个大小为 asize，另一个大小为 csize-asize
//否则，将 bp 放置为 asize 大小的块             
static void place(void *bp, size_t asize)
     /* $end mmplace-proto */
{
    size_t csize = GET_SIZE(HDRP(bp));   

    if ((csize - asize) >= (2*DSIZE)) { 
	PUT(HDRP(bp), PACK(asize, 1));
	PUT(FTRP(bp), PACK(asize, 1));
	bp = NEXT_BLKP(bp);
	PUT(HDRP(bp), PACK(csize-asize, 0));
	PUT(FTRP(bp), PACK(csize-asize, 0));
    }
    else { 
	PUT(HDRP(bp), PACK(csize, 1));
	PUT(FTRP(bp), PACK(csize, 1));
    }
}
/* $end mmplace */

/* 
 * find_fit - Find a fit for a block with asize bytes 
 */
/* $begin mmfirstfit */
/* $begin mmfirstfit-proto */
//参数：asize - 请求的内存大小，单位为字节
//返回值：若找到合适的空闲块，返回指向该块的指针；否则返回 NULL
//说明：在空闲链表中查找第一个大小大于等于 asize 的空闲块，若找到则返回该块的指针，否则返回 NULL
static void *find_fit(size_t asize)
/* $end mmfirstfit-proto */
{
/* $end mmfirstfit */

#ifdef NEXT_FIT 
    /* Next fit search */
    char *oldrover = rover;

    /* Search from the rover to the end of list */
    for ( ; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_BLKP(rover))
	if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
	    return rover;

    /* search from start of list to old rover */
    for (rover = heap_listp; rover < oldrover; rover = NEXT_BLKP(rover))
	if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
	    return rover;

    return NULL;  /* no fit found */
#else 
/* $begin mmfirstfit */
    /* First fit search */
    void *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
	if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
	    return bp;
	}
    }
    return NULL; /* No fit */
/* $end mmfirstfit */
#endif
}
//参数：bp - 指向块的指针
//返回值：无
//说明：打印块的大小、是否已分配、页眉和页脚
static void printblock(void *bp) 
{
    size_t hsize, halloc, fsize, falloc;

    checkheap(0);
    hsize = GET_SIZE(HDRP(bp));
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));
    falloc = GET_ALLOC(FTRP(bp));  

    if (hsize == 0) {
	printf("%p: EOL\n", bp);
	return;
    }

    /*  printf("%p: header: [%p:%c] footer: [%p:%c]\n", bp, 
	hsize, (halloc ? 'a' : 'f'), 
	fsize, (falloc ? 'a' : 'f')); */
}
//参数：bp - 指向块的指针
//返回值：无
//说明：检查块的页眉和页脚是否一致
//若不一致，打印错误信息
static void checkblock(void *bp) 
{
    //检查 1：8 字节对齐
    if ((size_t)bp % 8)
	printf("Error: %p is not doubleword aligned\n", bp);
    //检查 2：页眉和页脚是否一致
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
	printf("Error: header does not match footer\n");
}

/* 
 * checkheap - Minimal check of the heap for consistency 
 */
//参数：verbose - 是否打印堆信息    
//返回值：无
//说明：检查堆的页眉和页脚是否一致
//若不一致，打印错误信息
void checkheap(int verbose) 
{
    char *bp = heap_listp;

    if (verbose)
	printf("Heap (%p):\n", heap_listp);
    //检查 1：堆的页眉是否正确
    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))
	printf("Bad prologue header\n");
    checkblock(heap_listp);
//检查 2：堆的每个块的页眉和页脚是否一致
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
	if (verbose) 
	    printblock(bp);
	checkblock(bp);
    }

    if (verbose)
	printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
	printf("Bad epilogue header\n");
}


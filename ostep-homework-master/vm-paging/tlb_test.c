#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define PAGESIZE 4096          // 页大小（通常为4KB）
#define NUM_ITERATIONS 10000000 // 迭代次数

// 获取当前时间（微秒）
static inline long long get_time_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000000 + tv.tv_usec;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num_pages>\n", argv[0]);
        return 1;
    }
    
    int num_pages = atoi(argv[1]);
    int page_size = PAGESIZE;
    int jump = page_size / sizeof(int);  // 每页跳过的整数个数
    
    // 分配数组：num_pages 页 × 每页一个整数
    int *array = (int *)malloc(num_pages * page_size);
    if (!array) {
        perror("malloc failed");
        return 1;
    }
    
    // 初始化数组（确保所有页都被访问过）
    for (int i = 0; i < num_pages * jump; i += jump) {
        array[i] = i;
    }
    
    // 预热缓存
    for (int i = 0; i < num_pages * jump; i += jump) {
        array[i]++;
    }
    
    // 计时开始
    long long start = get_time_us();
    
    // 核心循环：反复访问每页的一个整数
    for (long long iter = 0; iter < NUM_ITERATIONS; iter++) {
        for (int i = 0; i < num_pages * jump; i += jump) {
            array[i]++;
        }
    }
    
    // 计时结束
    long long end = get_time_us();
    long long elapsed = end - start;
    
    // 计算平均每次访问的时间（纳秒）
    double avg_time_ns = (double)elapsed * 1000 / (NUM_ITERATIONS * num_pages);
    
    printf("Pages: %d, Avg access time: %.2f ns\n", num_pages, avg_time_ns);
    
    free(array);
    return 0;
}

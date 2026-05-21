// =============================================================================
// 基址-界限地址转换模拟器 (C语言版本)
// 
// 该程序模拟了使用基址寄存器(base)和界限寄存器(limit)进行地址转换的过程。
// 主要用于操作系统课程中讲解地址空间和内存管理的教学目的。
// 
// 地址转换原理：
//   - 虚拟地址(VA) + 基址(Base) = 物理地址(PA)
//   - 如果 VA >= 界限(Limit)，则发生段错误(Segmentation Violation)
// 
// 作者：自动生成
// 版本：1.0
// 日期：2024年
// =============================================================================

#include <stdio.h>      // 标准输入输出库
#include <stdlib.h>     // 标准库（包含 atoi, strtoll 等函数）
#include <string.h>     // 字符串处理库（包含 strcmp, strncpy 等函数）
#include <time.h>       // 时间库（用于随机数种子）
#include <ctype.h>      // 字符处理库（包含 tolower 函数）

// =============================================================================
// 常量定义
// =============================================================================
#define VERSION "1.0"           // 程序版本号
#define MAX_STR_LEN 256         // 字符串最大长度
#define RAND_MAX 0x7fffffff     // 伪随机数最大值

// =============================================================================
// 函数声明
// =============================================================================
long long convert(const char *size);
void print_usage(const char *prog_name);
unsigned int my_rand(unsigned int *seed);

// =============================================================================
// 主程序入口
// =============================================================================
int main(int argc, char *argv[]) {
    // ==================== 默认参数值 ====================
    int seed = 0;                      // 随机种子，用于生成可重复的随机数序列
    char asize_str[MAX_STR_LEN] = "1k"; // 地址空间大小（字符串形式，支持 k/m/g 单位）
    char psize_str[MAX_STR_LEN] = "16k"; // 物理内存大小（字符串形式，支持 k/m/g 单位）
    int num_addrs = 5;                 // 要生成的虚拟地址数量
    char base_str[MAX_STR_LEN] = "-1";  // 基址寄存器值（-1 表示自动生成）
    char limit_str[MAX_STR_LEN] = "-1"; // 界限寄存器值（-1 表示自动生成）
    int solve = 0;                     // 是否显示答案（0=学习模式，1=验证模式）

    // ==================== 解析命令行参数 ====================
    for (int i = 1; i < argc; i++) {
        // 帮助信息：显示用法说明
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        // -s 参数：设置随机种子
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = atoi(argv[++i]);
        }
        // -a 参数：设置地址空间大小（如 16, 64k, 32m, 1g）
        else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            strncpy(asize_str, argv[++i], MAX_STR_LEN - 1);
        }
        // -p 参数：设置物理内存大小（如 16, 64k, 32m, 1g）
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            strncpy(psize_str, argv[++i], MAX_STR_LEN - 1);
        }
        // -n 参数：设置要生成的虚拟地址数量
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            num_addrs = atoi(argv[++i]);
        }
        // -b 参数：设置基址寄存器值
        else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            strncpy(base_str, argv[++i], MAX_STR_LEN - 1);
        }
        // -l 参数：设置界限寄存器值
        else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            strncpy(limit_str, argv[++i], MAX_STR_LEN - 1);
        }
        // -c 参数：开启答案显示模式
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--compute") == 0) {
            solve = 1;
        }
        // 未知参数处理：打印错误信息并显示用法
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    // ==================== 打印参数信息 ====================
    printf("\nARG seed %d\n", seed);
    printf("ARG address space size %s\n", asize_str);
    printf("ARG phys mem size %s\n", psize_str);
    printf("\n");

    // ==================== 转换内存大小为字节数 ====================
    long long asize = convert(asize_str);  // 地址空间大小（字节）
    long long psize = convert(psize_str);  // 物理内存大小（字节）

    // ==================== 参数校验 ====================
    if (psize <= 1) {
        fprintf(stderr, "Error: must specify a non-zero physical memory size.\n");
        return 1;
    }
    if (asize == 0) {
        fprintf(stderr, "Error: must specify a non-zero address-space size.\n");
        return 1;
    }
    if (psize <= asize) {
        fprintf(stderr, "Error: physical memory size must be GREATER than "
                "address space size (for this simulation)\n");
        return 1;
    }

    // ==================== 转换基址和界限参数 ====================
    long long limit = convert(limit_str);  // 界限寄存器值
    long long base = convert(base_str);    // 基址寄存器值

    // ==================== 随机生成段寄存器值（如果未指定） ====================
    // 如果未指定界限，随机生成一个合理的值
    // 界限范围：[asize/4, asize/2]，确保有一定的地址空间可以访问
    if (limit == -1) {
        unsigned int rand_seed = seed;
        double r = (double)my_rand(&rand_seed) / RAND_MAX;
        limit = (long long)(asize / 4.0 + (asize / 4.0 * r));
    }

    // 如果未指定基址，随机生成一个合理的值
    // 确保基址+界限不超过物理内存大小，避免地址空间超出物理内存范围
    if (base == -1) {
        unsigned int rand_seed = seed + 1;
        do {
            double r = (double)my_rand(&rand_seed) / RAND_MAX;
            base = (long long)(psize * r);
        } while ((base + limit) >= psize);
    }

    // ==================== 打印段寄存器信息 ====================
    printf("Base-and-Bounds register information:\n");
    printf("\n");
    printf("  Base   : 0x%08llx (decimal %lld)\n", base, base);  // 十六进制和十进制显示
    printf("  Limit  : %lld\n", limit);
    printf("\n");

    // ==================== 最终检查：确保地址空间能放入物理内存 ====================
    if (base + limit > psize) {
        fprintf(stderr, "Error: address space does not fit into physical memory.\n");
        fprintf(stderr, "Base + Limit: %lld  Psize: %lld\n", base + limit, psize);
        return 1;
    }

    // ==================== 生成虚拟地址跟踪 ====================
    printf("Virtual Address Trace\n");
    unsigned int rand_seed = seed + 2;  // 使用不同的种子生成虚拟地址，避免与段寄存器生成冲突
    
    for (int i = 0; i < num_addrs; i++) {
        // 随机生成虚拟地址（范围：[0, asize)）
        double r = (double)my_rand(&rand_seed) / RAND_MAX;
        long long vaddr = (long long)(asize * r);

        if (!solve) {
            // 学习模式：不显示答案，让用户自己计算物理地址或判断越界
            printf("  VA %2d: 0x%08llx (decimal: %4lld) --> PA or segmentation violation?\n",
                   i, vaddr, vaddr);
        } else {
            // 验证模式：显示地址转换结果
            if (vaddr >= limit) {
                // 虚拟地址 >= 界限 → 段错误（越界访问）
                printf("  VA %2d: 0x%08llx (decimal: %4lld) --> SEGMENTATION VIOLATION\n",
                       i, vaddr, vaddr);
            } else {
                // 虚拟地址 < 界限 → 有效地址，计算物理地址
                // 核心公式：物理地址 = 虚拟地址 + 基址寄存器值
                long long paddr = vaddr + base;
                printf("  VA %2d: 0x%08llx (decimal: %4lld) --> VALID: 0x%08llx (decimal: %4lld)\n",
                       i, vaddr, vaddr, paddr, paddr);
            }
        }
    }

    printf("\n");

    // 如果是学习模式，打印提示信息
    if (!solve) {
        printf("For each virtual address, either write down the physical address it translates to\n");
        printf("OR write down that it is an out-of-bounds address (a segmentation violation). For\n");
        printf("this problem, you should assume a simple virtual address space of a given size.\n");
        printf("\n");
    }

    return 0;
}

// =============================================================================
// 将带单位的内存大小字符串转换为字节数
// 
// 支持的单位：
//   - 无单位：直接返回数值（字节）
//   - k/K：千字节（1k = 1024 bytes）
//   - m/M：兆字节（1m = 1024 * 1024 bytes）
//   - g/G：吉字节（1g = 1024 * 1024 * 1024 bytes）
// 
// @param size 内存大小字符串（如 "16", "64k", "32m", "1g"）
// @return 转换后的字节数
// =============================================================================
long long convert(const char *size) {
    // 参数校验
    if (size == NULL || strlen(size) == 0) {
        return 0;
    }

    // 使用 strtoll 解析数字部分
    char *endptr;
    long long value = strtoll(size, &endptr, 10);
    
    // 如果没有单位字符，直接返回数值
    if (*endptr == '\0') {
        return value;
    }

    // 根据单位进行转换
    char unit = tolower((unsigned char)*endptr);
    switch (unit) {
        case 'k':  // 千字节
            return value * 1024;
        case 'm':  // 兆字节
            return value * 1024 * 1024;
        case 'g':  // 吉字节
            return value * 1024 * 1024 * 1024;
        default:   // 未知单位，返回原值
            return value;
    }
}

// =============================================================================
// 打印程序用法说明
// 
// @param prog_name 程序名称（通常是 argv[0]）
// =============================================================================
void print_usage(const char *prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("\nOptions:\n");
    printf("  -h, --help            Show this help message and exit\n");
    printf("  -s SEED               Random seed (default: 0)\n");
    printf("  -a ASIZE              Address space size (e.g., 16, 64k, 32m, 1g)\n");
    printf("  -p PSIZE              Physical memory size (e.g., 16, 64k, 32m, 1g)\n");
    printf("  -n NUM                Number of virtual addresses to generate (default: 5)\n");
    printf("  -b BASE               Value of base register\n");
    printf("  -l LIMIT              Value of limit register\n");
    printf("  -c, --compute         Compute answers for me\n");
    printf("\nExample:\n");
    printf("  %s -s 1 -a 1k -p 16k -n 5\n", prog_name);
    printf("  %s -s 1 -a 1k -p 16k -n 5 -c\n", prog_name);
}

// =============================================================================
// 伪随机数生成器（线性同余生成器）
// 
// 使用标准的线性同余算法生成伪随机数，确保相同种子产生相同的随机数序列。
// 公式：next = (current * a + c) mod m
// 其中：a = 1103515245, c = 12345, m = 2^31
// 
// @param seed 指向种子变量的指针（输入/输出参数）
// @return 生成的伪随机数
// =============================================================================
unsigned int my_rand(unsigned int *seed) {
    *seed = (*seed * 1103515245 + 12345) & 0x7fffffff;
    return *seed;
}

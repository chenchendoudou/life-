#! /usr/bin/env python
# -*- coding: utf-8 -*-
# ==============================================================================
# 基址-界限地址转换模拟器
# 该程序模拟了使用基址寄存器(base)和界限寄存器(limit)进行地址转换的过程
# ==============================================================================

from __future__ import print_function
import sys
from optparse import OptionParser
import random
import math

# ------------------------------------------------------------------------------
# 兼容Python2和Python3的随机种子设置函数
# ------------------------------------------------------------------------------
def random_seed(seed):
    """设置随机数种子，兼容Python2和Python3"""
    try:
        random.seed(seed, version=1)  # Python3的方式
    except:
        random.seed(seed)             # Python2的方式
    return

# ------------------------------------------------------------------------------
# 内存大小单位转换函数
# 支持 k/K (千字节), m/M (兆字节), g/G (吉字节) 单位
# ------------------------------------------------------------------------------
def convert(size):
    """将带单位的内存大小字符串转换为字节数
    
    参数:
        size: 字符串，如 "16", "64k", "32m", "1g"
    
    返回:
        转换后的字节数（整数）
    """
    length = len(size)
    lastchar = size[length-1]  # 获取最后一个字符（单位）
    
    if (lastchar == 'k') or (lastchar == 'K'):
        m = 1024                    # 1k = 1024 bytes
        nsize = int(size[0:length-1]) * m
    elif (lastchar == 'm') or (lastchar == 'M'):
        m = 1024 * 1024             # 1m = 1024 * 1024 bytes
        nsize = int(size[0:length-1]) * m
    elif (lastchar == 'g') or (lastchar == 'G'):
        m = 1024 * 1024 * 1024      # 1g = 1024 * 1024 * 1024 bytes
        nsize = int(size[0:length-1]) * m
    else:
        nsize = int(size)           # 无单位，直接转换为整数
    
    return nsize


# ------------------------------------------------------------------------------
# 主程序入口
# ------------------------------------------------------------------------------
if __name__ == '__main__':
    # 创建命令行参数解析器
    parser = OptionParser()
    
    # 添加命令行参数
    parser.add_option('-s', '--seed',      default=0,     
                      help='随机种子',              action='store', type='int', dest='seed')
    parser.add_option('-a', '--asize',     default='1k',  
                      help='地址空间大小（如 16, 64k, 32m, 1g）',  
                      action='store', type='string', dest='asize')
    parser.add_option('-p', '--physmem',   default='16k', 
                      help='物理内存大小（如 16, 64k, 32m, 1g）',
                      action='store', type='string', dest='psize')
    parser.add_option('-n', '--addresses', default=5,     
                      help='要生成的虚拟地址数量',     
                      action='store', type='int', dest='num')
    parser.add_option('-b', '--b',         default='-1',  
                      help='基址寄存器的值',          
                      action='store', type='string', dest='base')
    parser.add_option('-l', '--l',         default='-1',  
                      help='界限寄存器的值',          
                      action='store', type='string', dest='limit')
    parser.add_option('-c', '--compute',   default=False,
                      help='自动计算答案（显示地址转换结果）',
                      action='store_true', dest='solve')

    # 解析命令行参数
    (options, args) = parser.parse_args()

    # 打印参数信息
    print('')
    print('ARG seed', options.seed)
    print('ARG address space size', options.asize)
    print('ARG phys mem size', options.psize)
    print('')

    # 设置随机种子
    random_seed(options.seed)
    
    # 转换地址空间大小和物理内存大小为字节数
    asize = convert(options.asize)
    psize = convert(options.psize)

    # 参数校验
    if psize <= 1:
        print('错误：必须指定非零的物理内存大小。')
        exit(1)

    if asize == 0:
        print('错误：必须指定非零的地址空间大小。')
        exit(1)

    if psize <= asize:
        print('错误：物理内存大小必须大于地址空间大小（本模拟要求）')
        exit(1)

    # --------------------------------------------------------------------------
    # 生成段寄存器的基址(base)和界限(limit)
    # --------------------------------------------------------------------------
    # 转换基址和界限参数
    limit = convert(options.limit)
    base  = convert(options.base)

    # 如果未指定界限，随机生成一个合理的界限值
    # 界限范围：[asize/4, asize/2]
    if limit == -1:
        limit = int(asize / 4.0 + (asize / 4.0 * random.random()))

    # 如果未指定基址，随机生成一个合理的基址值
    # 确保基址+界限不超过物理内存大小
    if base == -1:
        done = 0
        while done == 0:
            base = int(psize * random.random())  # 随机选择基址
            if (base + limit) < psize:           # 检查是否有足够空间
                done = 1

    # 打印段寄存器信息
    print('Base-and-Bounds register information:')
    print('')
    print('  Base   : 0x%08x (decimal %d)' % (base, base))
    print('  Limit  : %d' % (limit))
    print('')

    # 最终检查：确保地址空间能放入物理内存
    if base + limit > psize:
        print('错误：使用这些基址/界限值，地址空间无法放入物理内存。')
        print('Base + Limit:', base + limit, '  Psize:', psize)
        exit(1)

    # --------------------------------------------------------------------------
    # 生成虚拟地址跟踪
    # --------------------------------------------------------------------------
    print('Virtual Address Trace')
    
    for i in range(0, options.num):
        # 随机生成一个虚拟地址（范围：[0, asize)）
        vaddr = int(asize * random.random())
        
        if options.solve == False:
            # 不显示答案模式：让用户自己计算
            print('  VA %2d: 0x%08x (decimal: %4d) --> PA or segmentation violation?' 
                  % (i, vaddr, vaddr))
        else:
            # 显示答案模式：计算并显示地址转换结果
            paddr = 0
            if (vaddr >= limit):
                # 虚拟地址 >= 界限寄存器值 → 段错误（越界）
                print('  VA %2d: 0x%08x (decimal: %4d) --> SEGMENTATION VIOLATION' 
                      % (i, vaddr, vaddr))
            else:
                # 虚拟地址 < 界限寄存器值 → 有效地址
                # 物理地址 = 虚拟地址 + 基址寄存器值
                paddr = vaddr + base
                print('  VA %2d: 0x%08x (decimal: %4d) --> VALID: 0x%08x (decimal: %4d)' 
                      % (i, vaddr, vaddr, paddr, paddr))

    print('')

    # 如果不是显示答案模式，打印提示信息
    if options.solve == False:
        print('For each virtual address, either write down the physical address it translates to')
        print('OR write down that it is an out-of-bounds address (a segmentation violation). For')
        print('this problem, you should assume a simple virtual address space of a given size.')
        print('')

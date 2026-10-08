# =============================================================================
#  ECF 学习示例集 —— Makefile
# =============================================================================
#  本 Makefile 把 code/ecf/ecf-test/ 下六个主题子目录里的所有示例统一编译。
#
#  用法：
#    make               编译所有示例（生成 31 个可执行文件）
#    make clean         删除所有生成的可执行文件
#    make 02-fork/fork_basic      只编译某一个示例
#    make -n           # "干跑"，只打印要执行的命令而不实际执行
#    make -B           # 强制重新编译，忽略时间戳
#    make -j4          # 并行编译 4 个（在本 Makefile 里效果有限）
# =============================================================================

# -----------------------------------------------------------------------------
# 工具与编译选项
# -----------------------------------------------------------------------------
CC       = gcc              # C 编译器。也可以改成 clang / arm-linux-gcc 等。

CFLAGS   = -Wall -O0 -g     # 三个开关含义：
                            #   -Wall    打开编译器大部分警告（拼写错、未使用变量等）
                            #   -O0      关闭优化（学习阶段保留原貌，方便对照源码）
                            #   -g       生成调试符号，让 gdb 能定位行号
                            #
                            # 学习阶段就保持 -O0 -g；做性能实验时再换成 -O2。

INCLUDES = -I.              # 头文件搜索路径：当前目录。
                            # 所有 .c 都用 #include "csapp.h"，
                            # 而 csapp.h 就在 ecf-test/ 根目录，所以 -I. 就够。
                            # （之前试过 -I..，那是相对源代码所在目录而言的，
                            #   对 ecf-test/02-fork/forkprob7.c 来说 -I.. 会指到
                            #   code/ecf/，找不到 csapp.h。改成 -I. 后 OK。）

# -----------------------------------------------------------------------------
# 所有要生成的可执行文件（每个主题一个分组）
# -----------------------------------------------------------------------------
# 这里用 :=（立即展开）而不是 =（递归展开）。
# 后面用到 $(BINS) 时，Make 会先把这些字符串收集好。
#
# 路径都相对于当前目录（ecf-test/）。
#
# 重要：续行符 '\' 后面如果直接跟 '#' 注释，GNU Make 会把注释连同 '\' 一起
# 当作行尾吃掉，导致下一行变成"无规则"的孤立行而报错。
# 所以下面的做法是：每行先写 '\' 续行，把注释放到下一个完整行（# 开头）。
# 这里采用了另一种稳妥写法：先把所有路径写在多行块里，注释单独放出来。
BINS := 01-syscall-trap/demo_getpid
BINS += 01-syscall-trap/demo_forkasm

BINS += 02-fork/fork_basic
BINS += 02-fork/forkprob0
BINS += 02-fork/forkprob1
BINS += 02-fork/forkprob2
BINS += 02-fork/forkprob3
BINS += 02-fork/forkprob4
BINS += 02-fork/forkprob5
BINS += 02-fork/forkprob6
BINS += 02-fork/forkprob7
BINS += 02-fork/forkprob8

BINS += 03-waitpid/waitpid1
BINS += 03-waitpid/waitpid2
BINS += 03-waitpid/waitprob0
BINS += 03-waitpid/waitprob1
BINS += 03-waitpid/waitprob3

BINS += 04-signal/signal1
BINS += 04-signal/signal2
BINS += 04-signal/signal3
BINS += 04-signal/signal4
BINS += 04-signal/signalprob0
BINS += 04-signal/sigint1
BINS += 04-signal/kill
BINS += 04-signal/alarm
BINS += 04-signal/procmask1
BINS += 04-signal/procmask2
BINS += 04-signal/counterprob
BINS += 04-signal/restart
BINS += 04-signal/pkill_term

BINS += 05-setjmp/setjmp

BINS += 06-shellex/shellex

# 章节索引（方便阅读 BINS 列表时反查）
# 01-syscall-trap/demo_getpid    : § 8.1-8.3  syscall() 直接触发陷阱
# 01-syscall-trap/demo_forkasm   : § 8.1-8.3  fork() 内部也是一条 syscall
# 02-fork/fork_basic             : § 8.4      fork 教学版（带详细注释）
# 02-fork/forkprob0 .. forkprob8 : § 8.4      fork 思考题 0..8
# 03-waitpid/waitpid1, waitpid2  : § 8.4      waitpid 用法
# 03-waitpid/waitprob0..3        : § 8.4      waitpid 思考题
# 04-signal/signal1..signal4     : § 8.5      signal/sigaction 递进示例
# 04-signal/signalprob0          : § 8.5      signal 思考题
# 04-signal/sigint1              : § 8.5      SIGINT (Ctrl+C) 处理
# 04-signal/kill                 : § 8.5      kill() 显式发信号
# 04-signal/alarm                : § 8.5      alarm() 定时信号
# 04-signal/procmask1,2          : § 8.5      sigprocmask 演示
# 04-signal/counterprob          : § 8.5      信号与全局计数器
# 04-signal/restart              : § 8.5      SA_RESTART
# 04-signal/pkill_term           : § 8.5      SIGTERM 处理（pkill -9 不可捕获，用 SIGTERM 替代）
# 05-setjmp/setjmp               : § 8.6      非局部跳转
# 06-shellex/shellex             : § 8.4+8.5  综合：100 行简易 shell

# -----------------------------------------------------------------------------
# 默认目标
# -----------------------------------------------------------------------------
# `make` 不带参数时，等价于 `make all`。
# 依赖 $(BINS) 里所有 31 个文件，Make 会一项一项找规则去构建。
all: $(BINS)

# -----------------------------------------------------------------------------
# 模式规则（Pattern Rule）
# -----------------------------------------------------------------------------
# 这是 Make 最强大的特性之一。
# `%` 是通配符，`%: %.c` 表示：
#   "对任何形如 X 的目标，先找 X.c 作为依赖"。
#
# 例如要构建 `01-syscall-trap/demo_getpid`，
# Make 会自动去找 `01-syscall-trap/demo_getpid.c` 作为源文件。
#
# 自动变量：
#   $@   当前目标（这里是 01-syscall-trap/demo_getpid）
#   $<   第一个依赖（这里是 01-syscall-trap/demo_getpid.c）
#   $^   所有依赖（这里只有一个）
#
# 编译命令：
#   gcc -Wall -O0 -g -I. -o 01-syscall-trap/demo_getpid 01-syscall-trap/demo_getpid.c
#
# 注：
#   - 这条规则对所有 31 个示例都生效，所以 BINS 里只需要列"目标"路径。
#   - 如果某天想要某个特殊编译选项的源文件，可以单独写一条精确规则，
#     模式规则的优先级低于精确规则，Make 会优先用精确的。
%: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<

# -----------------------------------------------------------------------------
# 清理
# -----------------------------------------------------------------------------
clean:
	rm -f $(BINS)
	# -f 表示"文件不存在也不报错"，这样首次 make clean 不会失败。

# -----------------------------------------------------------------------------
# .PHONY：声明伪目标
# -----------------------------------------------------------------------------
# 伪目标不是文件名。如果当前目录里恰好有一个叫 `clean` 的文件，
# Make 会以为它已经"最新"了，跳过清理。
# 加上 .PHONY 后，无论有没有同名文件，`make clean` 都会执行。
.PHONY: all clean
# =============================================================================
# 小贴士
# =============================================================================
# 想单独编译某一个主题的所有文件？直接写主题前缀即可：
#   make 02-fork/fork_basic
# Make 会顺着模式规则找到 02-fork/fork_basic.c 并生成二进制。
#
# 想看 Make 真正要执行哪些命令？加 -n：
#   make -n 02-fork/fork_basic
# 这对调试 Makefile 自己非常有用。
# =============================================================================
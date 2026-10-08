# Ch7 · 链接

## 📌 核心概念

- 静态链接：编译时把库代码拷贝到可执行文件
- 动态链接：运行时由动态链接器（ld-linux）加载
- ELF 文件结构：ELF header、节（section）、程序头（program header）
- 常见节：`.text / .data / .bss / .rodata / .symtab / .strtab / .rel.text`
- 符号：全局/外部/本地、符号解析、重定位
- 强符号 vs 弱符号
- PIC（位置无关代码）、GOT（全局偏移表）、PLT（过程链接表）
- 库打桩：编译时（--wrap）、链接时（LD_PRELOAD）、运行时

## 🧠 命令速查

```bash
readelf -h a.out         # ELF 头
readelf -S a.out         # 节信息
readelf -s a.out         # 符号表
objdump -d -j .text a.out
objdump -r a.out         # 重定位项
nm a.out                 # 符号列表
ldd a.out                # 动态库依赖
```

## 🔑 例题归档

### 静态库的链接顺序（待填）
### PIC 与 GOT（待填）
### LD_PRELOAD 打桩（待填）

## 🐛 易错点

1. 多个全局符号同名时的强/弱解析规则
2. 静态库链接顺序很关键：`-la -lb` 与 `-lb -la` 可能不同
3. GOT 读取在装载时比 PIC 直接寻址多一跳

## 💡 心得

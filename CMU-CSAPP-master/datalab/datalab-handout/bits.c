/* 
 * CS:APP Data Lab 
 * CS:APP 数据实验室
 * 
 * <Please put your name and userid here>
 * <请在此处填写你的姓名和用户ID>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 * bits.c - 包含你对本实验解答的源文件。
 *          这是你将要提交给导师的文件。
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 * 警告: 不要包含 <stdio.h> 头文件; 它会干扰 dlc 编译器。
 * 你可以在不包含 <stdio.h> 的情况下仍然使用 printf 进行调试，
 * 尽管你可能会收到编译器警告。一般来说，忽略编译器警告不是好的做法，
 * 但在这种情况下是可以的。
 */

#if 0
/*
 * Instructions to Students:
 * 给学生的说明：
 *
 * STEP 1: Read the following instructions carefully.
 * 第一步: 仔细阅读以下说明。
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.
你将通过编辑此源文件中的函数集合来提供你对数据实验室的解答。

INTEGER CODING RULES:
整数编码规则:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
  用一行或多行实现该函数的 C 代码替换每个函数中的 "return" 语句。你的代码
  必须符合以下风格：
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      /* 简要描述你的实现是如何工作的 */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  每个 "Expr" 是一个仅使用以下内容的表达式：
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
     整数常量 0 到 255 (0xFF)，包含边界值。你不允许使用
     0xffffffff 这样的大常量。
  2. Function arguments and local variables (no global variables).
     函数参数和局部变量 (不使用全局变量)。
  3. Unary integer operations ! ~
     一元整数运算符 ! ~
  4. Binary integer operations & ^ | + << >>
     二元整数运算符 & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.
  有些题目会进一步限制允许使用的运算符集合。每个 "Expr" 可以由
  多个运算符组成。你不受限于每行一个运算符。

  You are expressly forbidden to:
  你被明确禁止：
  1. Use any control constructs such as if, do, while, for, switch, etc.
     使用任何控制结构，如 if, do, while, for, switch 等。
  2. Define or use any macros.
     定义或使用任何宏。
  3. Define any additional functions in this file.
     在此文件中定义任何额外的函数。
  4. Call any functions.
     调用任何函数。
  5. Use any other operations, such as &&, ||, -, or ?:
     使用任何其他运算符，如 &&, ||, -, 或 ?:
  6. Use any form of casting.
     使用任何形式的强制类型转换。
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.
     使用 int 以外的任何数据类型。这意味着你不能使用数组、结构体或联合体。

 
  You may assume that your machine:
  你可以假设你的机器：
  1. Uses 2s complement, 32-bit representations of integers.
     使用补码表示，32 位整数表示。
  2. Performs right shifts arithmetically.
     以算术方式执行右移。
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.
     当移位量小于 0 或大于 31 时，移位行为不可预测。


EXAMPLES OF ACCEPTABLE CODING STYLE:
可接受的编码风格示例:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   * pow2plus1 - 返回 2^x + 1，其中 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     /* 利用移位的能力来计算 2 的幂 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   * pow2plus4 - 返回 2^x + 4，其中 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     /* 利用移位的能力来计算 2 的幂 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES
浮点编码规则

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.
对于需要你实现浮点运算的题目，编码规则没有那么严格。
你可以使用循环和条件控制。你可以同时使用 int 和 unsigned。
你可以使用任意整数和无符号常量。你可以在 int 或 unsigned 数据上使用
任何算术、逻辑或比较运算符。

You are expressly forbidden to:
你被明确禁止：
  1. Define or use any macros.
     定义或使用任何宏。
  2. Define any additional functions in this file.
     在此文件中定义任何额外的函数。
  3. Call any functions.
     调用任何函数。
  4. Use any form of casting.
     使用任何形式的强制类型转换。
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
     使用 int 或 unsigned 以外的任何数据类型。这意味着你不能使用数组、
     结构体或联合体。
  6. Use any floating point data types, operations, or constants.
     使用任何浮点数据类型、运算或常量。


NOTES:
注意事项:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
     使用 dlc (数据实验室检查器) 编译器 (在讲义中描述) 来
     检查你的解答的合法性。
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
     每个函数有一个你在实现时允许使用的最大运算符数量
     (整数、逻辑或比较)。最大运算符数量由 dlc 检查。
     注意赋值 ('=') 不计数；你可以随意使用，不受处罚。
  3. Use the btest test harness to check your functions for correctness.
     使用 btest 测试框架来检查你的函数的正确性。
  4. Use the BDD checker to formally verify your functions
     使用 BDD 检查器来形式化验证你的函数。
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
     每个函数的最大运算符数量在每个函数的头部注释中给出。
     如果说明书和此文件中的最大运算符数量有任何不一致，
     以本文件为准。

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 第二步: 根据编码规则修改以下函数。
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   重要事项。为避免评分意外：
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *      使用 dlc 编译器检查你的解答是否符合编码规则。
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 *      使用 BDD 检查器形式化验证你的解答是否产生正确的答案。
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 * bitXor - 仅使用 ~ 和 & 实现 x^y
 *   Example: bitXor(4, 5) = 1
 *   示例: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   合法运算符: ~ &
 *   Max ops: 14
 *   最大运算符数: 14
 *   Rating: 1
 *   难度: 1
 */
int bitXor(int x, int y) {
  return ~( ~(x & ~y) & ~(~x & y) );
}
/* 
 * tmin - return minimum two's complement integer 
 * tmin - 返回最小的补码整数
 *   Legal ops: ! ~ & ^ | + << >>
 *   合法运算符: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   最大运算符数: 4
 *   Rating: 1
 *   难度: 1
 */
int tmin(void) {

  return 2;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 * isTmax - 如果 x 是最大的补码数则返回 1，否则返回 0
 *   Legal ops: ! ~ & ^ | +
 *   合法运算符: ! ~ & ^ | +
 *   Max ops: 10
 *   最大运算符数: 10
 *   Rating: 1
 *   难度: 1
 */
int isTmax(int x) {
  return 2;
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 * allOddBits - 如果所有奇数编号的位都设为 1 则返回 1
 *   其中位编号从 0 (最低有效位) 到 31 (最高有效位)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   示例 allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   合法运算符: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   最大运算符数: 12
 *   Rating: 2
 *   难度: 2
 */
int allOddBits(int x) {
  return 2;
}
/* 
 * negate - return -x 
 * negate - 返回 -x
 *   Example: negate(1) = -1.
 *   示例: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   合法运算符: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   最大运算符数: 5
 *   Rating: 2
 *   难度: 2
 */
int negate(int x) {
  return 2;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 * isAsciiDigit - 如果 0x30 <= x <= 0x39 (字符 '0' 到 '9' 的 ASCII 码) 则返回 1
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   示例: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   合法运算符: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   最大运算符数: 15
 *   Rating: 3
 *   难度: 3
 */
int isAsciiDigit(int x) {
  return 2;
}
/* 
 * conditional - same as x ? y : z 
 * conditional - 等同于 x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   示例: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   合法运算符: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   最大运算符数: 16
 *   Rating: 3
 *   难度: 3
 */
int conditional(int x, int y, int z) {
  return 2;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 * isLessOrEqual - 如果 x <= y 则返回 1，否则返回 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   示例: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   合法运算符: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   最大运算符数: 24
 *   Rating: 3
 *   难度: 3
 */
int isLessOrEqual(int x, int y) {
  return 2;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 * logicalNeg - 实现 ! 运算符，使用除 ! 之外的所有合法运算符
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   示例: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   合法运算符: ~ & ^ | + << >>
 *   Max ops: 12
 *   最大运算符数: 12
 *   Rating: 4 
 *   难度: 4 
 */
int logicalNeg(int x) {
  return 2;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 * howManyBits - 返回以补码表示 x 所需的最小位数
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  示例: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  合法运算符: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  最大运算符数: 90
 *  Rating: 4
 *  难度: 4
 */
int howManyBits(int x) {
  return 0;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 * floatScale2 - 返回浮点参数 f 表达 2*f 的位级等价形式
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   参数和结果都以 unsigned int 形式传递，但
 *   应将它们解释为单精度浮点值的位级表示。
 *   When argument is NaN, return argument
 *   当参数是 NaN 时，返回参数本身。
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   合法运算符: 任意整数/无符号运算，包括 ||, &&。也可以使用 if, while
 *   Max ops: 30
 *   最大运算符数: 30
 *   Rating: 4
 *   难度: 4
 */
unsigned floatScale2(unsigned uf) {
  return 2;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 * floatFloat2Int - 返回浮点参数 f 表达 (int) f 的位级等价形式
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   参数以 unsigned int 形式传递，但
 *   应将其解释为单精度浮点值的位级表示。
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   任何超出范围的值 (包括 NaN 和无穷大) 应返回 0x80000000u。
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   合法运算符: 任意整数/无符号运算，包括 ||, &&。也可以使用 if, while
 *   Max ops: 30
 *   最大运算符数: 30
 *   Rating: 4
 *   难度: 4
 */
int floatFloat2Int(unsigned uf) {
  return 2;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 * floatPower2 - 返回表达式 2.0^x (2.0 的 x 次幂) 对任意 32 位整数 x 的位级等价形式
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   返回的无符号值应具有与单精度浮点数 2.0^x 完全相同的位表示。
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *   如果结果太小无法表示为非规格化数，返回 0。如果太大，返回 +INF。
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   合法运算符: 任意整数/无符号运算，包括 ||, &&。也可以使用 if, while
 *   Max ops: 30 
 *   最大运算符数: 30
 *   Rating: 4
 *   难度: 4
 */
unsigned floatPower2(int x) {
    return 2;
}
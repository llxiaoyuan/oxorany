# [oxorany](https://github.com/llxiaoyuan/oxorany)
## 带有混淆的编译时任意常量加密

[![LICENSE](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![](https://img.shields.io/badge/C%2B%2B-14-brightgreen)]()
[![](https://img.shields.io/badge/OS-any-brightgreen)]()
[![](https://img.shields.io/badge/compiler-any-brightgreen)]()
[![](https://img.shields.io/badge/arch-any-brightgreen)]()

* [English](README.en.md)

### 介绍

> 我们综合了开源项目`ollvm`、`xorstr`一些实现思路，以及`c++14`标准中新加入的`constexpr`关键字和一些模板的知识，完成了编译时的任意常量的混淆`(可选)`和加密功能。

> 在C++14之前，我们如果要对程序中的常量进行保护，我们首先对常量进行加密操作，这里以字符串`"some_data_or_string"`逐字节`-1`为例，然后将加密后的数据"rnld^c\`s\`^nq^rsqhmf"，写到代码里，同时进行逐字节`+1`解密。

> 代码如下

```C++
char encrypted[] = {"rnld^c`s`^nq^rsqhmf"};
char key = 0x1;
for (size_t i = 0; i < strlen(encrypted); i++) {
	encrypted[i] += key;
}
//output: some_data_or_string
printf("%s\n", encrypted);
```

> 上述的方法只能在需要被保护的数据的数量比较少时使用，当数据量增大，繁琐的加密过程所占用的时间也会水涨船高，而且使得代码的可读性、可维护性大大降低。而且不可能为每一个数据都单独设计一个解密算法和key，使得通用的解密工具更易于编写。

> **随着`oxorany`的出现，上述过程将被改变**

### 🎨 特性
* **支持任意平台(`C++14`)，已在所有诸多编译器中进行了测试**
* **较高的可操作性，使用`__asm` `_emit`可进一步提高逆向难度**
* 所有的加密过程均在编译时完成
* 所有的解密过程均在栈内完成，无法通过运行时`dump`获得解密后的数据，不同于 [Armariris](https://github.com/GoSSIP-SJTU/Armariris)、[flounder](https://github.com/isrc-cas/flounder)
* 带有`伪造控制流`功能的解密算法
* 通过`编译优化`为每一个加密算法生成唯一的控制流
* 通过`__COUNTER__` 宏为每一个加密算法生成唯一的`key`
* 通过`__TIME__`宏动态产生`key`
* 代码经过**精心编写**，足以破坏堆栈以对抗`IDA` `F5`
* 基于堆栈变量的`不透明谓词`
* 模糊数据长度
* 由于解密算法的大部分代码不会被执行，所以对于效率的影响并不会特别大
* **解密算法的复杂度仍有提升空间**
* 因为`C++`中常量的`隐式转换`特性，某些常量可能需要强制类型转换
* **相当简单的使用方法**
* **不能保证数据会被内联到代码段**，[想要内联](https://github.com/llxiaoyuan/xorstr)

### 支持的数据类型

- [x] 字符串(`char*` `wchar_t*`)
- [x] 宏
- [x] 枚举
- [x] 字符(`char` `wchar_t`)
- [x] 指针(`NULL` `nullptr`)
- [x] 整数(`int8_t` `int16_t` `int32_t` `int64_t` `uint8_t` `uint16_t` `uint32_t` `uint64_t`)
- [ ] 浮点(`float` `double`)(**会保留原数据**)

### 支持的编译器

- [x] `msvc`
- [x] `clang`(`llvm`)(**支持叠加ollvm**)
- [x] `gcc`
- [x] `android ndk`(**支持安卓**)
- [x] `leetcode gcc`(**支持类似的云编译器**)
- [x] `wdk`(**支持Windows驱动程序**)
- [x] `...`

<br />

### 🚀 使用
```C++
#include <iostream>
//#define OXORANY_DISABLE_OBFUSCATION
#include "oxorany.h"

enum class MyEnum : int {
    first = 1,
    second = 2,
};

#define NUM_1 1

int main() {
    // output:
    // 1 1 2 c w 00000000 00000000 12 1234 12345678 1234567887654321 1.000000 2.000000
    // string u8 string wstring raw string raw wstring abcd
    printf(oxorany("%d %d %d %c %C %p %p %hhx %hx %x %llx %f %lf\n%s %s %S %s %S %s\n")  //string
           , oxorany(NUM_1)                                                           //macro
           , oxorany(MyEnum::first), oxorany(MyEnum::second)                          //enum
           , oxorany('c')                                                             //char
           , oxorany(L'w')                                                            //wchar_t
           , oxorany(NULL), oxorany(nullptr)                                          //pointer
           , oxorany(0x12)                                                            //int8_t
           , oxorany(0x1234)                                                          //int16_t
           , oxorany(0x12345678)                                                      //int32_t
           , oxorany(0x1234567887654321)                                              //int64_t
           , oxorany(1.0f)                                                            //float
           , oxorany(2.0)                                                             //double

           , oxorany("string")                                                        //string
           , oxorany(u8"u8 string")                                                   //u8 string
           , oxorany(L"wstring")                                                      //wstring
           , oxorany(R"(raw string)")                                                 //raw string
           , oxorany(LR"(raw wstring)")                                               //raw wstring
           , oxorany("\x61\x62\x63\x64")                                              //binary data
    );
    return oxorany(0);
}
```

<br />

### ⚙️ 需要强制类型转换的示例

> 0 error 0 warning

```C++
MessageBoxA(0, 0, 0, 0);
```

<br />

> 错误(活动)	E0167	"int" 类型的实参与 "HWND" 类型的形参不兼容

```C++
MessageBoxA(oxorany(0), 0, 0, 0);
```

<br />

> 出现上述问题的原因是因为`C/C++`中`0`的特殊性，因为它可以隐式转换到任意类型的指针，也和`NULL`的定义有关

```C++
#ifndef NULL
    #ifdef __cplusplus
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif
```

<br />

> 所以我们添加一个到`HWND`的强制类型转换就可以解决该问题
```C++
MessageBoxA(oxorany((HWND)0), 0, 0, 0);
```

<br />

### ⚙️ 在`wdk`中使用时需启用`__TIME__`宏
![image](https://user-images.githubusercontent.com/36320938/133402525-328e0515-6a30-473b-97cc-70905ca33606.png)

<br />

### `IDA`中的控制流程图

![image](https://user-images.githubusercontent.com/36320938/132527280-34c443b8-40b5-4b76-a35b-2629a1df087c.png)

<br />

### `编译优化`测试

> 这里是测试`编译优化`对控制流程图的影响，期望的结果是每一次编译都拥有不同的`控制流程图`

```C++
#include "oxorany.h"
int main() {
	return oxorany(0);
}
```

<br />

### ✅ 使用`msvc`多次编译后`IDA`中的`控制流程图`

![image](https://user-images.githubusercontent.com/36320938/132721095-7ef48f99-37ef-407e-bf14-0ba3e72f1e25.png)

<br />

### ✅ 使用`clang`多次编译后`IDA`中的`控制流程图`

![image](https://user-images.githubusercontent.com/36320938/132723406-dcf26a9e-9a12-4fa4-a0b7-4889861d7478.png)

<br />

### ✅ 使用`gcc`多次编译后`IDA`中的`控制流程图`

![image](https://user-images.githubusercontent.com/36320938/132799274-d816cc02-e913-43ab-b183-3a1c75fee5ce.png)

<br />

### ✅ 使用`android ndk`编译后`IDA`中的`控制流程图`

![image](https://user-images.githubusercontent.com/36320938/133132325-d70fe632-5e7e-407a-a42c-7594b788507a.png)

<br />

### ✅ 使用`leetcode gcc`进行测试 ([剑指 Offer 05. 替换空格](https://leetcode-cn.com/problems/ti-huan-kong-ge-lcof/comments/))

![S5(LFNXH~_KM6UH@L}U(CY6](https://user-images.githubusercontent.com/36320938/133314352-50d434a3-d5ae-4e0d-8504-1e1215be19e6.png)

<br />

### ✅ 使用`wdk`多次编译后`IDA`中的`控制流程图`

![image](https://user-images.githubusercontent.com/36320938/133409709-7e176557-439c-4988-91ee-219b35ab80e0.png)

<br />

### 不透明谓词

> 不透明：`opaque`  
> `★`来自拉丁语opacus,有阴影的，黑暗的，模糊的。

> 谓词：`predicate`  
> `★`来自拉丁语praedicare,预测，断言，声称，来自prae,在前，早于，dicare,说，声称，词源同diction.并引申诸相关词义。

> `不透明谓词`可以理解为`“无法确定结果的判断”`，词语本身并没有包含结果必为真或者必为假的含义，只是在这里使用了结果必为真的条件进行混淆。

> 代码中的`rand() % 2 == 0`实际上也是一个不透明谓词，因为我们无法确定它的结果，所以就无法确实程序是输出`hello`还是输出`world`

```C++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
	srand((unsigned int)time(NULL));
	if (rand() % 2 == 0) {
		printf("hello\n");
	}
	else {
		printf("world\n");
	}
	return 0;
}
```

<br />

> 但是换一种情况，这里我们创建了一个全局变量`zeor`，并赋初值为`0`，不去修改`zeor`的值或者在保证谓词结果恒定的情况下进行合理的修改，那么谓词`zeor < 1`就是恒成立的，同时又由于全局变量的天然的不透明性，编译器不会进行优化，所以我们就增加一个伪造的控制流，`无中生有`。我们可以在不可达的基本块内加入`任意代码`，这里我们添加了一个典中典`99乘法表`作为示例，`暗度陈仓`。

```C++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int zeor = 0;
int main() {
	if (zeor < 1) {
		printf("hello\n");
	}
	else {
		//unreachable
		for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
				printf("%d*%d=%2d\t", i, j, i * j);
			}
		    	printf("\n");
		}
	}
	return 0;
}
```

<br />

> 这里`copy`一下`ollvm`中的代码，`ASCII Picasso`

```C++
// Before :
// 	         	     entry
//      		       |
//  	    	  	 ______v______
//   	    		|   Original  |
//   	    		|_____________|
//             		       |
// 		       	       v
//		             return
//
// After :
//           		     entry
//             		       |
//            		   ____v_____
//      		  |condition*| (false)
//           		  |__________|----+
//           		 (true)|          |
//             		       |          |
//           		 ______v______    |
// 	            +-->|   Original* |   |
// 	            |   |_____________| (true)
// 	            |   (false)|    !-----------> return
// 	            |    ______v______    |
// 	            |   |   Altered   |<--!
// 	            |   |_____________|
// 	            |__________|
//
//  * The results of these terminator's branch's conditions are always true, but these predicates are
//    opacificated. For this, we declare two global values: x and y, and replace the FCMP_TRUE
//    predicate with (y < 10 || x * (x + 1) % 2 == 0) (this could be improved, as the global
//    values give a hint on where are the opaque predicates)
```

<br />

> `ollvm`中全局`x`、`y`的定义

```C++
      GlobalVariable 	* x = new GlobalVariable(M, Type::getInt32Ty(M.getContext()), false,
          GlobalValue::CommonLinkage, (Constant * )x1,
          *varX);
      GlobalVariable 	* y = new GlobalVariable(M, Type::getInt32Ty(M.getContext()), false,
          GlobalValue::CommonLinkage, (Constant * )y1,
          *varY);
```

<br />

> `ollvm`中不透明谓词`y < 10 || x * (x + 1) % 2 == 0`的实现，由`Instruction::Sub`可知，虽然注释是`x + 1`，但实际使用的确实`x - 1`，`问题不大，殊途同归`

```C++
        //if y < 10 || x*(x+1) % 2 == 0
        opX = new LoadInst ((Value *)x, "", (*i));
        opY = new LoadInst ((Value *)y, "", (*i));

        op = BinaryOperator::Create(Instruction::Sub, (Value *)opX,
            ConstantInt::get(Type::getInt32Ty(M.getContext()), 1,
              false), "", (*i));
        op1 = BinaryOperator::Create(Instruction::Mul, (Value *)opX, op, "", (*i));
        op = BinaryOperator::Create(Instruction::URem, op1,
            ConstantInt::get(Type::getInt32Ty(M.getContext()), 2,
              false), "", (*i));
        condition = new ICmpInst((*i), ICmpInst::ICMP_EQ, op,
            ConstantInt::get(Type::getInt32Ty(M.getContext()), 0,
              false));
        condition2 = new ICmpInst((*i), ICmpInst::ICMP_SLT, opY,
            ConstantInt::get(Type::getInt32Ty(M.getContext()), 10,
              false));
        op1 = BinaryOperator::Create(Instruction::Or, (Value *)condition,
            (Value *)condition2, "", (*i));

```


<br />

> 将我们上面的代码稍作调整，以展示`ollvm`的实现，这里的`x * (x + 1) % 2 == 0`，以为`x`和`x + 1`，必然是一个奇数一个偶数，根据奇偶性的运算法则可以得知`x * (x + 1)`的结果必然是偶数，所以`% 2 == 0`的判断将必然成立

```C++
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int x = 0;
int y = 0;
int main() {
	if (y < 10 || x * (x + 1) % 2 == 0) {
		printf("hello\n");
	}
	else {
		//unreachable
        	for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
                		printf("%d*%d=%2d\t", i, j, i * j);
			}
            		printf("\n");
        	}
	}
	return 0;
}
```

<br />

### 实现
> 受到`ollvm`中`伪造控制流`功能的启发，我们创建了两个全局变量`x`、`y`，并赋初值为`0`，作为实现不透明谓词的基础

![image](https://user-images.githubusercontent.com/36320938/132540802-06b63425-acc8-4da8-b9d7-de5886587f42.png)

<br />

> 由于栈环境的复杂性，我们将全局变量`x`、`y`分别赋值给两个局部变量`stack_x`、`stack_y`，以提高逆向的难度

![image](https://user-images.githubusercontent.com/36320938/132541176-02f4f8a7-0b80-4b2a-b584-7658f954a003.png)

<br />

> 我们在函数的很多位置创建了`label`，使用`stack_x`、`stack_y`进行恒为真的判断进行混淆，在无法到达的基本快内添加`goto label`以尽可能得对基本块进行拆分。我们在多处对解密后的数据`decrypted`使用错误的`key`进行解密，使得真实的`key`在众多的错误的`key`中难以被识别，`乱花渐欲迷人眼，浅草才能没马蹄`

![image](https://user-images.githubusercontent.com/36320938/132542465-c9495bde-c34f-468b-ae0f-b9ab79959bba.png)

<br />

> 生成带有范围限制的随机数，因为这里可以出现相同的值，同时又因为编译优化的存在，重复的条件会被消除，这使得我们每一次的编译，都拥有不尽相同的控制流程图

![image](https://user-images.githubusercontent.com/36320938/132543102-c7c59806-6f34-4f60-b5cf-59abdfa79048.png)

<br />

> 我们在无法到达的基本快内加入非法的栈操作再加上代码中经过精心分配的控制流使得`IDA`的栈帧分析失败，以对抗`F5`

![image](https://user-images.githubusercontent.com/36320938/132544334-27a63575-35b0-4b52-ac12-9079a984c2bf.png)

<br />

> 我们在将数据按`16`字节对齐并加上一定的随机值以模糊数据长度，这可能会浪费一点空间

![image](https://user-images.githubusercontent.com/36320938/132553464-d8ef7b64-c4a7-4a36-9250-51062751a8d1.png)

<br />

> 我们在将`xor`替换为一种更加复杂的实现方式，以提高逆向的难度

![image](https://user-images.githubusercontent.com/36320938/132621379-81796348-23d1-4549-99b7-55e4aa87f0eb.png)

<br />

> 使用`__TIME__`宏实现每一次编译都拥有唯一的`key`

![image](https://user-images.githubusercontent.com/36320938/132704045-7510c6df-f2db-4e9b-99b0-ca80aa871aed.png)

<br />

> 带有范围限制的随机数产生器，使得`不透明谓词`相似于正常的`谓词`

![image](https://user-images.githubusercontent.com/36320938/132704535-10761dda-61e7-47b3-95a4-e2439d483532.png)

<br />

> 综上所述，在`oxorany`的帮助下，软件的安全性将会得到进一步的提高

### 参考
+ [孤挺花（Armariris）-- 由上海交通大学密码与计算机安全实验室维护的LLVM混淆框架](https://github.com/GoSSIP-SJTU/Armariris)
+ [PLCT实验室维护的ollvm分支](https://github.com/isrc-cas/flounder)
+ [高度矢量化的 c++17 编译时字符串加密](https://github.com/JustasMasiulis/xorstr)

### Github
https://github.com/llxiaoyuan/oxorany

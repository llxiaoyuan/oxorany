# oxorany
## A heavily obfuscated c++14 compile time any constant encryption.

### 介绍

> 我们综合了开源项目`ollvm`、`xorstr`一些实现思路，以及`c++14`标准中新加入的`constexpr`关键字和一些模板的知识，完成了编译时的任意常量的混淆加密，所支持的数据类型基本涵盖了`C++`的全部类型的常量，`字符串`、`宏`、`枚举`、`整数`、`浮点`等已知的数据类型

<br />

### 特性
* 编译时的任意常量的混淆加密
* 带有`伪造控制流`功能的解密算法
* 通过`编译优化`为每一个加密算法生成唯一的控制流
* 通过`__COUNTER__` 宏为每一个加密算法生成唯一的密钥
* 通过`__TIME__`宏动态产生密钥
* 破坏堆栈以对抗`IDA` `F5`
* 基于堆栈变量的`不透明谓词`
* 模糊数据长度
* 由于解密算法的大部分代码不会被执行，所以对于效率的影响并不会特别大
* 解密算法的复杂度仍有提升空间
* 因为`C++`中常量的`隐式转换`特性，某些常量可能需要强制类型转换
* 简单易用

<br />

### 使用
```C++
#include <iostream>
#include "oxorany.h"

enum class MyEnum : int {
	first = 1,
	second = 2,
};

#define NUM_1 1

int main() {
        // output:
        // 1 1 2 12 1234 12345678 1234567887654321 1.000000 2.000000
        // string wstring raw string raw wstring
	printf(oxorany("%d %d %d %hhx %hx %x %llx %f %lf\n%s %S %s %S\n")  //string
            , oxorany(NUM_1)                                               //macro
            , oxorany(MyEnum::first), oxorany(MyEnum::second)              //enum
            , oxorany((uint8_t)0x12)                                       //uint8_t
            , oxorany((uint16_t)0x1234)                                    //uint16_t
            , oxorany((uint32_t)0x12345678)                                //uint32_t
            , oxorany((uint64_t)0x1234567887654321)                        //uint64_t
            , oxorany(1.0f)                                                //float
            , oxorany(2.0)                                                 //double
            , oxorany("string")                                            //string
            , oxorany(L"wstring")                                          //wstring
            , oxorany(R"(raw string)")                                     //raw string
            , oxorany(LR"(raw wstring)")                                   //raw wstring
	);
	return oxorany(0);
}
```

<br />

### IDA中的控制流程图
![image](https://user-images.githubusercontent.com/36320938/132527280-34c443b8-40b5-4b76-a35b-2629a1df087c.png)

<br />

### 不透明谓词

不透明：opaque

★来自拉丁语opacus,有阴影的，黑暗的，模糊的。

谓词：predicate

★来自拉丁语praedicare,预测，断言，声称，来自prae,在前，早于，dicare,说，声称，词源同diction.并引申诸相关词义。


### 实现
> 受到`ollvm`中`伪造控制流`功能的启发，我们创建了两个全局变量`x`、`y`，并赋初值为`0`，作为实现不透明谓词的基础
![image](https://user-images.githubusercontent.com/36320938/132540802-06b63425-acc8-4da8-b9d7-de5886587f42.png)

<br />

> 由于栈环境的复杂性，我们将全局变量`x`、`y`分别赋值给两个局部变量`stack_x`、`stack_y`，以提高逆向的难度

![image](https://user-images.githubusercontent.com/36320938/132541176-02f4f8a7-0b80-4b2a-b584-7658f954a003.png)

<br />

> 我们在函数的很多位置创建了`label`，使用`stack_x`、`stack_y`进行恒为真的判断进行混淆，在无法到达的基本快内添加`goto label`以尽可能得对基本块进行拆分。我们在多处对解密后的数据`decrypted`使用错误的`key`进行解密，使得真实的`key`在众多的错误的`key`中难以被识别

![image](https://user-images.githubusercontent.com/36320938/132542465-c9495bde-c34f-468b-ae0f-b9ab79959bba.png)

<br />

> 生成带有范围限制的随机数，因为这里可以出现相同的值，同时又因为编译优化的存在，重复的条件会被优化掉，这使得我们每一次的编译，都拥有不尽相同的控制流程图

![image](https://user-images.githubusercontent.com/36320938/132543102-c7c59806-6f34-4f60-b5cf-59abdfa79048.png)

<br />

> 我们在无法到达的基本快内加入非法的栈操作使得`IDA`的栈帧分析失败，以对抗`F5`

![image](https://user-images.githubusercontent.com/36320938/132544334-27a63575-35b0-4b52-ac12-9079a984c2bf.png)

<br />

> 我们在将数据按`16`字节对齐并加上一定的随机值以模糊数据长度，这可能会浪费一点空间

![image](https://user-images.githubusercontent.com/36320938/132553464-d8ef7b64-c4a7-4a36-9250-51062751a8d1.png)

<br />

> 我们在将`xor`替换为一种更加复杂的实现方式，以提高逆向的难度

![image](https://user-images.githubusercontent.com/36320938/132621379-81796348-23d1-4549-99b7-55e4aa87f0eb.png)

<br />

> 综上，在`oxorany`的帮助下，任何单纯依赖`IDA`静态分析的反向工程将变得不切实际

<br />

### 参考
+ [PLCT实验室维护的ollvm分支](https://github.com/isrc-cas/flounder)
+ [高度矢量化的 c++17 编译时字符串加密](https://github.com/JustasMasiulis/xorstr)

<br />

### Github
https://github.com/llxiaoyuan/oxorany

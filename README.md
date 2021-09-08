# oxorany
## any constant encryption with obfuscation in compile time

### Features
* any constant encryption with obfuscation in `compile time`
* bogus control flow in decryption algorithm 
* unique control flow for each encryption algorithm by `compiler optimization`
* unique key for each encryption algorithm by `__COUNTER__` macro
* generate dynamic key by `__TIME__` macro
* stack frame destroy for anti `IDA` `F5`
* opaque predicate on the stack

### Usage
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

### CFG in IDA
![image](https://user-images.githubusercontent.com/36320938/132527280-34c443b8-40b5-4b76-a35b-2629a1df087c.png)

<br />

### Implement
> 受到ollvm中伪造控制流功能的启发，我们创建了两个全局变量`x`、`y`，并赋初值为`0`，作为实现不透明谓词的基础

![image](https://user-images.githubusercontent.com/36320938/132540802-06b63425-acc8-4da8-b9d7-de5886587f42.png)

<br />

> 由于堆栈环境的复杂性，我们将全局变量`x`、`y`分别赋值给两个局部变量`stack_x`、`stack_y`，以提高逆向的难度

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

> 综上，在`oxorany`的帮助下，依赖`IDA`静态分析将变得不切实际




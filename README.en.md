# oxorany
## A heavily obfuscated c++14 compile time any constant encryption.

[![LICENSE](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![](https://img.shields.io/badge/C%2B%2B-14-brightgreen)]()
[![](https://img.shields.io/badge/OS-any-brightgreen)]()
[![](https://img.shields.io/badge/compiler-any-brightgreen)]()
[![](https://img.shields.io/badge/arch-any-brightgreen)]()

* [简体中文](README.md)

### Description

> We integrated some implementation ideas from open source projects `ollvm` and `xorstr`, as well as the new `constexpr` keyword in the `c++14` standard and some template knowledge, to complete the compile-time obfuscation encryption of arbitrary constants.

> Before C++14, if we want to protect the constants in the program, we first encrypt the constants, here we take the string `"some_data_or_string"` byte by byte `-1` as an example, and then write the encrypted data "rnld^c\`s\`^nq^rsqhmf" to the code, while doing byte by byte `+1` decryption.

> Code show as below

```C++
char encrypted[] = {"rnld^c`s`^nq^rsqhmf"};
char key = 0x1;
for (size_t i = 0; i < strlen(encrypted); i++) {
	encrypted[i] += key;
}
//output: some_data_or_string
printf("%s\n", encrypted);
```


> The above method can only be used when the amount of data to be protected is relatively small, and when the amount of data increases, the time taken by the tedious encryption process will also rise, and it makes the readability and maintainability of the code greatly reduced. And it is not possible to design a separate decryption algorithm and key for each data, which makes a general decryption tool easier to write.

> **With the advent of `oxorany`, the above process will be changed**

### Features
* Obfuscated encryption of any constants at compile time
* All the decryption process is done inside the stack, and the decrypted data cannot be obtained through runtime `dump`，unlike [Armariris](https://github.com/hcygithub/Armariris)、[flounder](https://github.com/isrc-cas/flounder)
* Decryption algorithm with `Bogus Control Flow` like `ollvm`
* Generate a unique control flow for each encryption algorithm through `compile optimization`
* Generate a unique `key` for each encryption algorithm with the `__COUNTER__` macro
* Dynamically generate `key` via the `__TIME__` macro
* **The code has been crafted** so that can Destroying the stack to anti `IDA` `F5`
* Stack variable based `Opaque Predicate`
* Fuzzy data length
* Since most of the code for the decryption algorithm is not executed, the impact on efficiency is not particularly significant
* **The complexity of the decryption algorithm can be improve**
* Because of the `implicit conversion` feature of constants in `C++`, some constants may require forced type conversion
* Easy to use, tested in `msvc`, `clang`, `gcc`

### Data types supported

- [x] String(`wchar_t*` `char*`)
- [x] Macro
- [x] Enume
- [x] Integer(`int8_t` `int16_t` `int32_t` `int64_t` `uint8_t` `uint16_t` `uint32_t` `uint64_t`)
- [x] Floating point(`float` `double`)

### Compilers Supported

- [x] `msvc`
- [x] `clang`(`llvm`)
- [x] `gcc`
- [x] `...`

<br />

### Usage
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

### Need Cast

> 0 error 0 warning

```C++
MessageBoxA(0, 0, 0, 0);
```

<br />

> error(active)	E0167 Real parameters of type "int" are incompatible with formal parameters of type "HWND"

```C++
MessageBoxA(oxorany(0), 0, 0, 0);
```

<br />

> The reason for the above problem is due to the peculiarity of `0` in `C/C++`, because it can be implicitly converted to a pointer of any type, and is also related to the definition of `NULL`

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

> So we add a forced type conversion to `HWND` to solve the problem
```C++
MessageBoxA(oxorany((HWND)0), 0, 0, 0);
```

<br />

### `Control Flow Graph` in `IDA`

![image](https://user-images.githubusercontent.com/36320938/132527280-34c443b8-40b5-4b76-a35b-2629a1df087c.png)

<br />

### `Compilation Optimization` test

```C++
#include "oxorany.h"
int main() {
	return oxorany(0);
}
```

<br />

### `Control Flow Graph` in `IDA` after multiple compilation using `msvc`

![image](https://user-images.githubusercontent.com/36320938/132721095-7ef48f99-37ef-407e-bf14-0ba3e72f1e25.png)

<br />

### `Control Flow Graph` in `IDA` after multiple compilation using `clang`

![image](https://user-images.githubusercontent.com/36320938/132723406-dcf26a9e-9a12-4fa4-a0b7-4889861d7478.png)

<br />

### `Control Flow Graph` in `IDA` after multiple compilation using `gcc`

![image](https://user-images.githubusercontent.com/36320938/132799274-d816cc02-e913-43ab-b183-3a1c75fee5ce.png)

<br />

### Opaque predicate

> The `opaque predicate` can be understood as `"the judgment of the result cannot be determined"`，The words themselves do not contain the meaning that the result must be true or must be false, but only the condition that the result must be true is used here for obfuscation

> The `rand() % 2 == 0` in the code is actually an opaque predicate, because we can't determine its result, so we can't be sure whether the program is outputting `hello` or `world`

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

> But in another case, here we create a global variable `zeor` and assign an initial value of `0`, without modifying the value of `zeor` or making reasonable modifications to ensure that the result of the predicate is constant, then the predicate `zeor < 1` is constant, and at the same time the compiler will not optimize due to the natural opacity of global variables, so we add a forged to the control flow. We can add `any code` inside an unreachable basic block, and here we add a `99 multiplication table` as an example.。

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

> Here `copy` the code from `ollvm`，`ASCII Picasso`

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

> Definition of global `x`, `y` in ollvm`

```C++
      GlobalVariable 	* x = new GlobalVariable(M, Type::getInt32Ty(M.getContext()), false,
          GlobalValue::CommonLinkage, (Constant * )x1,
          *varX);
      GlobalVariable 	* y = new GlobalVariable(M, Type::getInt32Ty(M.getContext()), false,
          GlobalValue::CommonLinkage, (Constant * )y1,
          *varY);
```

<br />

> The implementation of the opaque predicate `y < 10 || x * (x + 1) % 2 == 0` in `ollvm` is shown by `Instruction::Sub`, which, although annotated with `x + 1`, actually uses `x - 1`

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

> Adjusting our code above slightly to show the implementation of `ollvm`, here `x * (x + 1) % 2 == 0`, thinking that `x` and `x + 1`, must be an odd number and an even number, according to the operation of parity we can learn that the result of `x * (x + 1)` must be even, so the judgment of `% 2 == 0` will necessarily hold

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

### Implemention
> Inspired by the `Bogus Control Flow` function in `ollvm`, we created two global variables `x` and `y` and assigned initial values of `0` as the basis for implementing opaque predicates

![image](https://user-images.githubusercontent.com/36320938/132540802-06b63425-acc8-4da8-b9d7-de5886587f42.png)

<br />

> Due to the complexity of the stack environment, we assign the global variables `x` and `y` to two local variables `stack_x` and `stack_y` respectively to make the inverse more difficult

![image](https://user-images.githubusercontent.com/36320938/132541176-02f4f8a7-0b80-4b2a-b584-7658f954a003.png)

<br />

> We created `label` in many positions of the function, used `stack_x`, `stack_y` to judge the constant to be true for confusion, and added `goto label` in the basic block that could not be reached to remove the basic block as much as possible point. We use the wrong key to decrypt the decrypted data `decrypted` in many places, so that the real key is difficult to identify among the many wrong keys.

![image](https://user-images.githubusercontent.com/36320938/132542465-c9495bde-c34f-468b-ae0f-b9ab79959bba.png)

<br />

> Generate random numbers with range limition, since the same values can occur here, while duplicate conditions are removed by compilation optimization because of the existence of compilation optimization, which makes us have a different control flow diagram for each compilation

![image](https://user-images.githubusercontent.com/36320938/132543102-c7c59806-6f34-4f60-b5cf-59abdfa79048.png)

<br />

> We add illegal stack operations within the unreachable basic fast to make `IDA`'s stack frame analysis fail against `F5`

![image](https://user-images.githubusercontent.com/36320938/132544334-27a63575-35b0-4b52-ac12-9079a984c2bf.png)

<br />

> We are aligning the data by `16` bytes and adding a certain random value to obscure the data length, which may waste a little space

![image](https://user-images.githubusercontent.com/36320938/132553464-d8ef7b64-c4a7-4a36-9250-51062751a8d1.png)

<br />

> We are replacing `xor` with a more complex implementation to make the inversion more difficult

![image](https://user-images.githubusercontent.com/36320938/132621379-81796348-23d1-4549-99b7-55e4aa87f0eb.png)

<br />

> Use the `__TIME__` macro to implement a different `key` for each compilation

![image](https://user-images.githubusercontent.com/36320938/132704045-7510c6df-f2db-4e9b-99b0-ca80aa871aed.png)

<br />

> Random number generator with range restrictions, making `opaque predicates` similar to normal `predicates`

![image](https://user-images.githubusercontent.com/36320938/132704535-10761dda-61e7-47b3-95a4-e2439d483532.png)

<br />

> To sum up, with the help of `oxorany`, the security of the software will be further improved

### Reference
+ [Armariris -- LLVM obfuscation framework maintained by the Laboratory of Cryptography and Computer Security, Shanghai Jiao Tong University](https://github.com/hcygithub/Armariris)
+ [PLCT labs maintain the collvm branch](https://github.com/isrc-cas/flounder)
+ [heavily vectorized c++17 compile time string encryption](https://github.com/JustasMasiulis/xorstr)

### Github
https://github.com/llxiaoyuan/oxorany

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


### CFG in IDA
![image](https://user-images.githubusercontent.com/36320938/132527280-34c443b8-40b5-4b76-a35b-2629a1df087c.png)





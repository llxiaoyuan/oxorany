#include <iostream>
#define OXORANY_DISABLE_OBFUSCATION
//use OXORANY_USE_BIT_CAST for remove float double src data
#define OXORANY_USE_BIT_CAST
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
    //printf(oxorany("%d %d %d %c %C %p %p %hhx %hx %x %llx %f %lf\n%s %s %S %s %S %s\n")  //string
    //       , oxorany(NUM_1)                                                           //macro
    //       , oxorany(MyEnum::first), oxorany(MyEnum::second)                          //enum
    //       , oxorany('c')                                                             //char
    //       , oxorany(L'w')                                                            //wchar_t
    //       , oxorany(NULL), oxorany(nullptr)                                          //pointer
    //       , oxorany(0x12)                                                            //int8_t
    //       , oxorany(0x1234)                                                          //int16_t
    //       , oxorany(0x12345678)                                                      //int32_t
    //       , oxorany(0x1234567887654321)                                              //int64_t
    //       , oxorany_flt(1.0f)                                                            //float
    //       , oxorany_flt(2.0)                                                             //double

    //       , oxorany("string")                                                        //string
    //       , oxorany(u8"u8 string")                                                   //u8 string
    //       , oxorany(L"wstring")                                                      //wstring
    //       , oxorany(R"(raw string)")                                                 //raw string
    //       , oxorany(LR"(raw wstring)")                                               //raw wstring
    //       , oxorany("\x61\x62\x63\x64")                                              //binary data
    //);
    return (int)oxorany_flt(3.0f) + (int)oxorany_flt(6.0);
}

#include <stdio.h>
#include "oxorany_include.h"

enum class MyEnum : int {
    first = 1,
    second = 2,
};

#define NUM_1 1

int main() {
    // output:
    // 1 1 2 c w 00000000 00000000 12 1234 12345678 1234567887654321 1.000000 2.000000
    // string u8 string wstring raw string raw wstring abcd
    //printf(WRAPPER_MARCO("%d %d %d %c %C %p %p %hhx %hx %x %llx %f %lf\n%s %s %S %s %S %s\n")  //string
    //       , WRAPPER_MARCO(NUM_1)                                                           //macro
    //       , WRAPPER_MARCO(MyEnum::first), WRAPPER_MARCO(MyEnum::second)                          //enum
    //       , WRAPPER_MARCO('c')                                                             //char
    //       , WRAPPER_MARCO(L'w')                                                            //wchar_t
    //       , WRAPPER_MARCO(NULL), WRAPPER_MARCO(nullptr)                                          //pointer
    //       , WRAPPER_MARCO(0x12)                                                            //int8_t
    //       , WRAPPER_MARCO(0x1234)                                                          //int16_t
    //       , WRAPPER_MARCO(0x12345678)                                                      //int32_t
    //       , WRAPPER_MARCO(0x1234567887654321)                                              //int64_t
    //       , WRAPPER_MARCO_FLT(1.0f)                                                            //float
    //       , WRAPPER_MARCO_FLT(2.0)                                                             //double

    //       , WRAPPER_MARCO("string")                                                        //string
    //       , WRAPPER_MARCO(u8"u8 string")                                                   //u8 string
    //       , WRAPPER_MARCO(L"wstring")                                                      //wstring
    //       , WRAPPER_MARCO(R"(raw string)")                                                 //raw string
    //       , WRAPPER_MARCO(LR"(raw wstring)")                                               //raw wstring
    //       , WRAPPER_MARCO("\x61\x62\x63\x64")                                              //binary data
    //);

    WRAPPER_MARCO_DEFINE_ARRAY(char, name_a, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    WRAPPER_MARCO_DEFINE_ARRAY(wchar_t, name_w, L"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
    printf("%s\n", name_a);
    printf("%S\n", name_w);

    printf(WRAPPER_MARCO("%d\n"), WRAPPER_MARCO(3) + (int)WRAPPER_MARCO_FLT(6.0));
    return 0;
}

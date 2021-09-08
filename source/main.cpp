#include <iostream>
#include "oxorany.h"

enum class MyEnum : int {
	first = 1,
	second = 2,
};

#define NUM_1 1

int main() {
	printf(oxorany("%d %d %d %hhx %hx %x %llx %f %lf\n%s %S %s %S\n")	//string
		   , oxorany(NUM_1)												//macro
		   , oxorany(MyEnum::first), oxorany(MyEnum::second)			//enum
		   , oxorany((uint8_t)0x12)										//uint8_t
		   , oxorany((uint16_t)0x1234)									//uint16_t
		   , oxorany((uint32_t)0x12345678)								//uint32_t
		   , oxorany((uint64_t)0x1234567887654321)						//uint64_t
		   , oxorany(1.0f)												//float
		   , oxorany(2.0)												//double
		   , oxorany("string")											//string
		   , oxorany(L"wstring")										//wstring
		   , oxorany(R"(raw string)")									//raw string
		   , oxorany(LR"(raw wstring)")									//raw wstring
	);
	return oxorany(0);
}


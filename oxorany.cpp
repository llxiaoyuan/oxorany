#include "oxorany.h"

size_t& _oxor_any_::X() {
	static size_t x = 0; 
	return x;
}

size_t& _oxor_any_::Y() {
	static size_t y = 0; 
	return y;
}

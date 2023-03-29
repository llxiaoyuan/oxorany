#ifndef OXORANY_PREDEF_H
#define OXORANY_PREDEF_H

#define OXORANY_DISABLE_OBFUSCATION
//use OXORANY_USE_BIT_CAST for remove float double src data
#define OXORANY_USE_BIT_CAST

#include "oxorany.h"

//to change Marco in a easy way
#define WRAPPER_MARCO oxorany
#define WRAPPER_MARCO_FLT oxorany_flt

template <typename T>
static OXORANY_FORCEINLINE void copy_string_without_result(T* target, const T* source)
{
	while (*source) {
		*target = *source;
		++source;
		++target;
	}
	*target = 0;
}

#define WRAPPER_MARCO_DEFINE_ARRAY(type,name,s) type name[sizeof(s)]; copy_string_without_result(name, WRAPPER_MARCO(s))

#endif
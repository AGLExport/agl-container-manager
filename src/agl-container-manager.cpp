#include <stdio.h>
#include <exsample.hpp>

extern "C" int exsample_c_func(void);


int main(void)
{
	printf("abcd\n");

	exsample_c_func();

#ifdef	_USER_DEBUG_
	printf("USERDEBUG_C");
#endif	//_USER_DEBUG_

#ifdef	_PRINTF_DEBUG_
	printf("PRINTFDEBUG_C");
#endif	//_PRINTF_DEBUG_

	return 0;
}

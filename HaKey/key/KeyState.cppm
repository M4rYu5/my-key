module;

#if LINUX
#include <asm-generic/int-ll64.h>
#endif

export module KeyState;

namespace HaKey {

	export enum KeyState 
#if LINUX
	: __s32 
#endif
	{
		Up = 0,
		Down = 1,
		Repeat = 2
	};

}
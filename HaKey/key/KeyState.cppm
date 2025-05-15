module;

#include "shared/Platform.h"

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
		/// @brief Windows doesn't support Repeating chanracters.
		Repeat = PLATFORM_VALUE(2, 1) // windows doesn't differentiate between down and repeat
	};

}
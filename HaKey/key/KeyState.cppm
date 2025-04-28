module;

#include <asm-generic/int-ll64.h>

export module KeyState;

namespace HaKey {

	export enum KeyState : __s32 {
		Up = 0,
		Down = 1,
		Repeat = 2
	};

}
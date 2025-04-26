export module Core:State;

namespace HaKey::Core {

	//namespace HaKeyState {
	//	const int DOWN = 1;
	//}
	export enum class KeyState {
		Down,
		Up,
		Repeat
	};

}
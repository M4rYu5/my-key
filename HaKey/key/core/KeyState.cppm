export module Core:KeyState;

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
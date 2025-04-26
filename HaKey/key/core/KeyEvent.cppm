export module Core:Event;

import :State;
import :VK;

namespace HaKey::Core {

	export class KeyEvent {
	public:
		VKey key;
		KeyState state;
	};

}
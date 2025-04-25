export module Key:Event;

import :State;
import :VK;

namespace Key::Core {

	export class KeyEvent {
	public:
		VKey key;
		KeyState state;
	};

}
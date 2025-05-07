
export module Core:Key;

import KeyCode;
import KeyState;

namespace HaKey::Core {

	export struct Key {
	public:
		KeyCode code;
		KeyState state;

		Key(KeyCode code, KeyState state){
			this->code = code;
			this->state = state;
		}

		Key(int code, int state){
			this->code = (KeyCode)code;
			this->state = (KeyState)state;
		}
	};

}
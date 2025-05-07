
export module Core:Key;

import VKey;
import KeyState;

namespace HaKey::Core {

	export struct Key {
	public:
		VKey key_code;
		KeyState state;

		Key(VKey key_code, KeyState state){
			this->key_code = key_code;
			this->state = state;
		}

		Key(int key_code, int state){
			this->key_code = (VKey)key_code;
			this->state = (KeyState)state;
		}
	};

}
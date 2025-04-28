
export module Core:KeyEvent;

import VKey;

namespace HaKey::Core {

	export struct KeyEvent {
	public:
		VKey key_code;
		int state;

		KeyEvent(VKey key_code, int state){
			this->key_code = key_code;
			this->state = state;
		}

		KeyEvent(int key_code, int state){
			this->key_code = (VKey)key_code;
			this->state = state;
		}
	};

}
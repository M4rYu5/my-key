export module Core:Event;

// import :State;
// import :VK;

namespace HaKey::Core {

	export struct KeyEvent {
	public:
		int key_code;
		int state;

		KeyEvent(int key_code, int state){
			this->key_code = key_code;
			this->state = state;
		}
	};

}
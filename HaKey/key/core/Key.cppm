
export module Core:Key;

import KeyCode;
import KeyState;

namespace HaKey::Core {

	export struct Key {
	public:
		KeyCode code;
		KeyState state;


		Key(KeyCode code = KeyCode::NONE, KeyState state = KeyState::Up){
			this->code = code;
			this->state = state;
		}

		/// @param code integer value of KeyCode
		/// @param state integer value of KeyState
		Key(int code, int state){
			this->code = (KeyCode)code;
			this->state = (KeyState)state;
		}

		
        inline bool Is(KeyCode key)
        {
            return code == key;
        }

        inline bool IsUp()
        {
            return state == KeyState::Up;
        }

		inline bool IsDown()
        {
            return state == KeyState::Down;
        }

        inline bool IsDownOrRepeat()
        {
            return state == KeyState::Down || state == KeyState::Repeat;
        }
		
	};

}
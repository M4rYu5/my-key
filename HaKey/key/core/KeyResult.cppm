module;

#include <vector>

export module Core:KeyResult;

import :KeyEvent;
import VKey;

namespace HaKey::Core {

	export class KeyResult {
	public:
		bool suppress_original;
		std::vector<KeyEvent> keys;

		void AddPressed(VKey k){
			keys.push_back(KeyEvent(k, 1));
		}

		void AddReleased(VKey k){
			keys.push_back(KeyEvent(k, 0));
		}

		void AddFullKey(VKey k){
			// push
			keys.push_back(KeyEvent(k, 1));
			// release
			keys.push_back(KeyEvent(k, 0));
		}

		void Clear(){
			suppress_original = false;
			keys.clear();
		}
	};
}
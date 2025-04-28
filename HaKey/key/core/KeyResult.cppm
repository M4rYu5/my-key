module;

#include <vector>

export module Core:KeyResult;

import :KeyEvent;

namespace HaKey::Core {

	export class KeyResult {
	public:
		bool suppress_original;
		std::vector<KeyEvent> keys;
	};

}
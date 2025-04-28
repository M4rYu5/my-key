module;

#include <vector>

export module Core:Result;

import VKey;

namespace HaKey::Core {

	export class KeyResult {
	public:
		bool suppress_original;
		std::vector<VKey> keys;
	};

}
module;

#include <vector>

export module Core:Result;

import :VK;

namespace HaKey::Core {

	export class KeyResult {
	public:
		bool suppress_original;
		std::vector<VKey> out_keys;
	};

}
export module Core:Result;

import :VK;

namespace HaKey::Core {

	export class KeyResult {
	public:
		bool suppress_original;
		VKey out_keys[];
	};

}
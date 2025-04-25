export module Key:Result;

import :VK;

namespace Key::Core {

	export class KeyResult {
	public:
		bool suppress_original;
		VKey out_keys[];
	};

}
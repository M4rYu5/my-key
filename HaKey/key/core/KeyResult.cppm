module;

#include <vector>

export module Core:KeyResult;

import :KeyEvent;
import VKey;

namespace HaKey::Core
{

	export class KeyResult
	{
	public:
		bool suppress_original;
		std::vector<KeyEvent> keys;

		KeyResult()
		{
			keys.reserve(100);
		}

		void AddPressed(VKey k)
		{
			keys.emplace_back(k, 1);
		}

		void AddReleased(VKey k)
		{
			keys.emplace_back(k, 0);
		}

		void AddFullKey(VKey k)
		{
			// push
			keys.emplace_back(k, 1);
			// release
			keys.emplace_back(k, 0);
		}

		void Clear()
		{
			suppress_original = false;
			keys.clear();
		}
	};
}
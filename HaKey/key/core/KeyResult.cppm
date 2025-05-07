module;

#include <vector>

export module Core:KeyResult;

import :Key;
import VKey;
import KeyState;

namespace HaKey::Core
{

	export class KeyResult
	{
	public:
		bool suppress_original;
		std::vector<Key> keys;

		KeyResult()
		{
			keys.reserve(100);
		}

		inline void AddKey(VKey k, KeyState state)
		{
			keys.emplace_back(k, state);
		}

		inline void AddPressed(VKey k)
		{
			AddKey(k, KeyState::Down);
		}

		inline void AddRepeated(VKey k)
		{
			AddKey(k, KeyState::Repeat);
		}

		inline void AddReleased(VKey k)
		{
			AddKey(k, KeyState::Up);
		}

		inline void AddFullKey(VKey k)
		{
			// push
			keys.emplace_back(k, 1);
			// release
			keys.emplace_back(k, 0);
		}

		inline void Clear()
		{
			suppress_original = false;
			keys.clear();
		}
	};
}
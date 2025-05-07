module;

#include <vector>

export module Core:KeyResult;

import :Key;
import KeyCode;
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

		inline void AddKey(KeyCode k, KeyState state)
		{
			keys.emplace_back(k, state);
		}

		inline void AddPressed(KeyCode k)
		{
			AddKey(k, KeyState::Down);
		}

		inline void AddRepeated(KeyCode k)
		{
			AddKey(k, KeyState::Repeat);
		}

		inline void AddReleased(KeyCode k)
		{
			AddKey(k, KeyState::Up);
		}

		inline void AddFullKey(KeyCode k)
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
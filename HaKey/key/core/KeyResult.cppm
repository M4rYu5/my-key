module;

#include <vector>
#include <unordered_set>
#include <span>

export module Core:KeyResult;

import :Key;
import KeyCode;
import KeyState;

namespace HaKey::Core
{

	export class KeyResult
	{
	private:
		std::unordered_set<KeyCode> keys_press_once;
	
	public:
		bool suppress_original = false;
		std::vector<Key> keys;

		KeyResult()
		{
			keys.reserve(100);
			keys_press_once.reserve(3);
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

		/// @brief Only the first call for each key will be added.
		/// Does reset with Clear().
		inline void AddFullKeyOnce(KeyCode key_code){
			auto [_, inserted] = keys_press_once.emplace(key_code);
			if (inserted){
				AddFullKey(key_code);
			}
		}

		inline void Clear()
		{
			suppress_original = false;
			keys.clear();
			keys_press_once.clear();
		}
	};
}
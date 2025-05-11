module;

#include <unordered_set>
#include <unordered_map>

export module Core:KeyShare;

import :Key;
import KeyCode;
import KeyState;

namespace HaKey::Core
{

    export struct KeyShare
    {
    private:
        std::unordered_map<KeyCode, int> generated_hotkey;
        std::unordered_set<KeyCode> keys_state_down; // currently pressed or KeyState::Down
    public:
        KeyShare()
        {
            keys_state_down.reserve(10);
        }

        /// @brief Increment shared resource that keeps track of keys that have generated a HotKey between layers;
        void AddGeneratedHotKey(KeyCode tracked_key_code)
        {
            generated_hotkey[tracked_key_code] += 1;
        }

        /// @brief Return wether or not the given key_code generated a HotKey
        bool GeneratedHotKey(KeyCode key_code) const
        {
            return generated_hotkey.find(key_code) != generated_hotkey.end();
        }

        bool IsKeyDown(KeyCode code)
        {
            return keys_state_down.contains(code);
        }

        void OnEarlyKeyEvent(Key &key)
        {
            if (key.IsDown())
            {
                keys_state_down.emplace(key.code);
            }
            else if (key.IsUp())
            {
                keys_state_down.erase(key.code);
            }
        }

        void OnLateKeyEvent(Key &key)
        {
            if (key.IsUp())
            {
                auto item = generated_hotkey.find(key.code);
                if (item != generated_hotkey.end())
                {
                    generated_hotkey.erase(key.code);
                }
            }
        }
    };

}
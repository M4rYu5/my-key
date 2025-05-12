module;

#include <memory>
#include <unordered_set>

export module Layers:PropagateKey;

import :BaseHotKeyLayer;
import Core;
import KeyCode;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    /// @brief This Layer is used by default in KeyDispatcher, allowing keys to pass (with modifiers) even if a HotKey is currently held.
    export class PropagateKey : public BaseHotKeyLayer
    {
    private:
        std::unordered_set<KeyCode> active_suppressed_mod_keys;

    public:
        void OnKey(Core::KeyContext& context) override
        {
            CheckActiveKeySuppressedRelease(context.key);
            next(context);
            CheckActiveKeySuppressed(context);

            SendWithSuppressedMod(context);
        }
        
        inline bool IsModifierKey(KeyCode code){
            return (
                code == KeyCode::LEFTSHIFT ||
                code == KeyCode::RIGHTSHIFT ||
                code == KeyCode::LEFTCTRL ||
                code == KeyCode::RIGHTCTRL ||
                code == KeyCode::LEFTALT ||
                code == KeyCode::RIGHTALT
            );
        }

        inline void SendWithSuppressedMod(Core::KeyContext& context)
        {
            if (!context.IsKeySuppressed())
            {
                for (auto key_code : active_suppressed_mod_keys)
                {
                    Send(key_code, KeyState::Down, context, KeyCode::NONE);
                }
                // context.result.keys.insert(context.result.keys.begin(), context.key);
                Send(context.key.code, context.key.state, context, KeyCode::NONE);
                for (auto key_code : active_suppressed_mod_keys)
                {
                    Send(key_code, KeyState::Up, context, KeyCode::NONE);
                }
            }
        }

        inline void CheckActiveKeySuppressedRelease(Core::Key& key)
        {
            if (key.IsUp() && IsModifierKey(key.code))
            {
                active_suppressed_mod_keys.erase(key.code);
            }
        }

        inline void CheckActiveKeySuppressed(Core::KeyContext& context)
        {
            if (context.key.IsDown() && context.IsKeySuppressed() && IsModifierKey(context.key.code))
            {
                active_suppressed_mod_keys.insert(context.key.code);
            }
        }
    };
} // namespace HaKey::Layers

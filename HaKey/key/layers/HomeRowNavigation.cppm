module;

#include <memory>
#include <unordered_set>

export module Layers:HomeRowNavigation;

import Core;
import VKey;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class HomeRowNavigation : public Core::KeyChainHandler
    {
    private:
        bool _caps_hold = false;
        bool _generated_hotkey = false;
        std::unordered_set<VKey> active_keys;

    public:
        void OnKey(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result) override
        {
            // update
            HotKeyUpdate(key, result);
            if (!_caps_hold)
            {
                next(key, result);
                return;
            }

            KeySwap(key, result);

            // propagate
            next(key, result);
        }

    private:
        inline void HotKeyUpdate(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            if (IsKeyUp(VKey::CAPSLOCK, key))
            {
                if (!_generated_hotkey)
                {
                    result->AddFullKey(VKey::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(VKey::CAPSLOCK, key))
            {
                _caps_hold = true;
                result->suppress_original = true;
            }
        }

        void ReleaseActiveKeys(std::shared_ptr<Core::KeyResult> result)
        {
            for (VKey key : active_keys)
            {
                result->AddKey(key, KeyState::Up);
            }
            active_keys.clear();
        }

        inline void KeySwap(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            // CAPSLOCK & s::LShift
            if (IsKey(VKey::S, key))
                Send(VKey::LEFTSHIFT, key.state, result);

            // CAPSLOCK & d::LControl
            if (IsKey(VKey::D, key))
                Send(VKey::LEFTCTRL, key.state, result);

            // CAPSLOCK & f::LAlt
            if (IsKey(VKey::F, key))
                Send(VKey::LEFTALT, key.state, result);

            // CAPSLOCK & j::Left
            if (IsKey(VKey::J, key))
                Send(VKey::LEFT, key.state, result);

            // CAPSLOCK & k::Down
            if (IsKey(VKey::K, key))
                Send(VKey::DOWN, key.state, result);

            // CAPSLOCK & l::Right
            if (IsKey(VKey::L, key))
                Send(VKey::RIGHT, key.state, result);

            // CAPSLOCK & i::Up
            if (IsKey(VKey::I, key))
                Send(VKey::UP, key.state, result);

            // CAPSLOCK & ;::End
            if (IsKey(VKey::SEMICOLON, key))
                Send(VKey::END, key.state, result);

            // CAPSLOCK & h::Home
            if (IsKey(VKey::H, key))
                Send(VKey::HOME, key.state, result);

            // CAPSLOCK & o::Delete
            if (IsKey(VKey::O, key))
                Send(VKey::DELETE, key.state, result);

            // often I press CapsLock and Space, after End hotkey (caps + ;),
            // which will keep the CapsLock ON after relese,
            // this will prevent this behaviour
            // CAPSLOCK & SPACE::SPACE
            if (IsKey(VKey::SPACE, key))
                Send(VKey::SPACE, key.state, result);
        }

        inline void Send(VKey key, KeyState state, std::shared_ptr<Core::KeyResult> result)
        {
            result->AddKey(key, state);
            _generated_hotkey = true;
            result->suppress_original = true;

            if (state == KeyState::Up)
            {
                active_keys.erase(key);
            }
            else if (state == KeyState::Down)
            {
                active_keys.insert(key);
            }
        }

        inline bool IsKey(VKey key, Core::KeyEvent event)
        {
            return event.key_code == key;
        }

        inline bool IsKeyUp(VKey key, Core::KeyEvent event)
        {
            return event.key_code == key && event.state == KeyState::Up;
        }

        inline bool IsKeyDownOrRepeat(VKey key, Core::KeyEvent event)
        {
            return event.key_code == key && (event.state == KeyState::Down || event.state == KeyState::Repeat);
        }
    };
} // namespace HaKey::Layers

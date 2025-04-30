module;

#include <memory>

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
                _caps_hold = false;
                _generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(VKey::CAPSLOCK, key))
            {
                _caps_hold = true;
                result->suppress_original = true;
            }
        }

        inline void KeySwap(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            // CAPSLOCK & s::LShift
            if (IsKeyDownOrRepeat(VKey::S, key))
                Send(VKey::LEFTSHIFT, result);

            // CAPSLOCK & d::LControl
            if (IsKeyDownOrRepeat(VKey::D, key))
                Send(VKey::LEFTCTRL, result);

            // CAPSLOCK & f::LAlt
            if (IsKeyDownOrRepeat(VKey::F, key))
                Send(VKey::LEFTALT, result);

            // CAPSLOCK & j::Left
            if (IsKeyDownOrRepeat(VKey::J, key))
                Send(VKey::LEFT, result);

            // CAPSLOCK & k::Down
            if (IsKeyDownOrRepeat(VKey::K, key))
                Send(VKey::DOWN, result);

            // CAPSLOCK & l::Right
            if (IsKeyDownOrRepeat(VKey::L, key))
                Send(VKey::RIGHT, result);

            // CAPSLOCK & i::Up
            if (IsKeyDownOrRepeat(VKey::I, key))
                Send(VKey::UP, result);

            // CAPSLOCK & ;::End
            if (IsKeyDownOrRepeat(VKey::SEMICOLON, key))
                Send(VKey::END, result);

            // CAPSLOCK & h::Home
            if (IsKeyDownOrRepeat(VKey::H, key))
                Send(VKey::HOME, result);

            // CAPSLOCK & o::Delete
            if (IsKeyDownOrRepeat(VKey::O, key))
                Send(VKey::DELETE, result);

            // often I press CapsLock and Space, after End hotkey (caps + ;),
            // which will keep the CapsLock ON after relese,
            // this will prevent this behaviour
            // CAPSLOCK & SPACE::SPACE
            if (IsKeyDownOrRepeat(VKey::SPACE, key))
                Send(VKey::SPACE, result);
        }

        inline void Send(VKey key, std::shared_ptr<Core::KeyResult> result)
        {
            result->AddFullKey(key);
            _generated_hotkey = true;
            result->suppress_original = true;
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
} // namespace HaKey

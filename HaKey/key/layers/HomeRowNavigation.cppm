module;

#include <memory>
#include <unordered_set>

export module Layers:HomeRowNavigation;

import Core;
import KeyCode;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class HomeRowNavigation : public Core::KeyChainHandler
    {
    private:
        bool _caps_hold = false;
        bool _generated_hotkey = false;
        std::unordered_set<KeyCode> active_keys;

    public:
        void OnKey(Core::Key key, std::shared_ptr<Core::KeyResult> result) override
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
        inline void HotKeyUpdate(Core::Key key, std::shared_ptr<Core::KeyResult> result)
        {
            if (IsKeyUp(KeyCode::CAPSLOCK, key))
            {
                if (!_generated_hotkey)
                {
                    result->AddFullKey(KeyCode::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(KeyCode::CAPSLOCK, key))
            {
                _caps_hold = true;
                result->suppress_original = true;
            }
        }

        void ReleaseActiveKeys(std::shared_ptr<Core::KeyResult> result)
        {
            for (KeyCode key : active_keys)
            {
                result->AddKey(key, KeyState::Up);
            }
            active_keys.clear();
        }

        inline void KeySwap(Core::Key key, std::shared_ptr<Core::KeyResult> result)
        {
            // CAPSLOCK & s::LShift
            if (IsKey(KeyCode::S, key))
                Send(KeyCode::LEFTSHIFT, key.state, result);

            // CAPSLOCK & d::LControl
            if (IsKey(KeyCode::D, key))
                Send(KeyCode::LEFTCTRL, key.state, result);

            // CAPSLOCK & f::LAlt
            if (IsKey(KeyCode::F, key))
                Send(KeyCode::LEFTALT, key.state, result);

            // CAPSLOCK & j::Left
            if (IsKey(KeyCode::J, key))
                Send(KeyCode::LEFT, key.state, result);

            // CAPSLOCK & k::Down
            if (IsKey(KeyCode::K, key))
                Send(KeyCode::DOWN, key.state, result);

            // CAPSLOCK & l::Right
            if (IsKey(KeyCode::L, key))
                Send(KeyCode::RIGHT, key.state, result);

            // CAPSLOCK & i::Up
            if (IsKey(KeyCode::I, key))
                Send(KeyCode::UP, key.state, result);

            // CAPSLOCK & ;::End
            if (IsKey(KeyCode::SEMICOLON, key))
                Send(KeyCode::END, key.state, result);

            // CAPSLOCK & h::Home
            if (IsKey(KeyCode::H, key))
                Send(KeyCode::HOME, key.state, result);

            // CAPSLOCK & o::Delete
            if (IsKey(KeyCode::O, key))
                Send(KeyCode::DELETE, key.state, result);

            // often I press CapsLock and Space, after End hotkey (caps + ;),
            // which will keep the CapsLock ON after relese,
            // this will prevent this behaviour
            // CAPSLOCK & SPACE::SPACE
            if (IsKey(KeyCode::SPACE, key))
                Send(KeyCode::SPACE, key.state, result);
        }

        inline void Send(KeyCode key, KeyState state, std::shared_ptr<Core::KeyResult> result)
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

        inline bool IsKey(KeyCode key, Core::Key event)
        {
            return event.code == key;
        }

        inline bool IsKeyUp(KeyCode key, Core::Key event)
        {
            return event.code == key && event.state == KeyState::Up;
        }

        inline bool IsKeyDownOrRepeat(KeyCode key, Core::Key event)
        {
            return event.code == key && (event.state == KeyState::Down || event.state == KeyState::Repeat);
        }
    };
} // namespace HaKey::Layers

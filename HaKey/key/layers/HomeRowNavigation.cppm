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
        void OnKey(Core::KeyContext& context) override
        {
            // update
            HotKeyUpdate(context.key, context.result);
            if (!_caps_hold)
            {
                next(context);
                return;
            }

            KeySwap(context.key, context.result);

            // propagate
            next(context);
        }

    private:
        inline void HotKeyUpdate(Core::Key key, Core::KeyResult& result)
        {
            if (key.IsUp(KeyCode::CAPSLOCK))
            {
                if (!_generated_hotkey)
                {
                    result.AddFullKeyOnce(KeyCode::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _generated_hotkey = false;
            }
            else if (key.IsDownOrRepeat(KeyCode::CAPSLOCK))
            {
                _caps_hold = true;
                result.suppress_original = true;
            }
        }

        void ReleaseActiveKeys(Core::KeyResult& result)
        {
            for (KeyCode key : active_keys)
            {
                result.AddKey(key, KeyState::Up);
            }
            active_keys.clear();
        }

        inline void KeySwap(Core::Key key, Core::KeyResult& result)
        {
            // CAPSLOCK & s::LShift
            if (key.Is(KeyCode::S))
                Send(KeyCode::LEFTSHIFT, key.state, result);

            // CAPSLOCK & d::LControl
            if (key.Is(KeyCode::D))
                Send(KeyCode::LEFTCTRL, key.state, result);

            // CAPSLOCK & f::LAlt
            if (key.Is(KeyCode::F))
                Send(KeyCode::LEFTALT, key.state, result);

            // CAPSLOCK & j::Left
            if (key.Is(KeyCode::J))
                Send(KeyCode::LEFT, key.state, result);

            // CAPSLOCK & k::Down
            if (key.Is(KeyCode::K))
                Send(KeyCode::DOWN, key.state, result);

            // CAPSLOCK & l::Right
            if (key.Is(KeyCode::L))
                Send(KeyCode::RIGHT, key.state, result);

            // CAPSLOCK & i::Up
            if (key.Is(KeyCode::I))
                Send(KeyCode::UP, key.state, result);

            // CAPSLOCK & ;::End
            if (key.Is(KeyCode::SEMICOLON))
                Send(KeyCode::END, key.state, result);

            // CAPSLOCK & h::Home
            if (key.Is(KeyCode::H))
                Send(KeyCode::HOME, key.state, result);

            // CAPSLOCK & o::Delete
            if (key.Is(KeyCode::O))
                Send(KeyCode::DELETE, key.state, result);

            // often I press CapsLock and Space, after End hotkey (caps + ;),
            // which will keep the CapsLock ON after relese,
            // this will prevent this behaviour
            // CAPSLOCK & SPACE::SPACE
            if (key.Is(KeyCode::SPACE))
                Send(KeyCode::SPACE, key.state, result);
        }

        inline void Send(KeyCode code, KeyState state, Core::KeyResult& result)
        {
            result.AddKey(code, state);
            _generated_hotkey = true;
            result.suppress_original = true;

            if (state == KeyState::Up)
            {
                active_keys.erase(code);
            }
            else if (state == KeyState::Down)
            {
                active_keys.insert(code);
            }
        }

    };
} // namespace HaKey::Layers

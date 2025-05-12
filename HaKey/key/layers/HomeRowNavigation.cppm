module;

#include <memory>
#include <unordered_set>

export module Layers:HomeRowNavigation;

import :BaseHotKeyLayer;
import Core;
import KeyCode;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class HomeRowNavigation : public BaseHotKeyLayer
    {
    public:
        void OnKey(Core::KeyContext& context) override
        {
            // update
            HotKeyUpdate(context, KeyCode::CAPSLOCK);

            if (!context.key.Is(KeyCode::CAPSLOCK) && context.state.IsKeyDown(KeyCode::CAPSLOCK)){
                KeySwap(context);
            }

            // propagate
            next(context);
        }

    private:
        inline void KeySwap(Core::KeyContext& context)
        {
            Core::Key& key = context.key;
            Core::KeyResult& result = context.result;

            // CAPSLOCK & s::LShift
            if (key.Is(KeyCode::S))
                Send(KeyCode::LEFTSHIFT, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & d::LControl
            if (key.Is(KeyCode::D))
                Send(KeyCode::LEFTCTRL, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & f::LAlt
            if (key.Is(KeyCode::F))
                Send(KeyCode::LEFTALT, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & j::Left
            if (key.Is(KeyCode::J))
                Send(KeyCode::LEFT, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & k::Down
            if (key.Is(KeyCode::K))
                Send(KeyCode::DOWN, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & l::Right
            if (key.Is(KeyCode::L))
                Send(KeyCode::RIGHT, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & i::Up
            if (key.Is(KeyCode::I))
                Send(KeyCode::UP, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & ;::End
            if (key.Is(KeyCode::SEMICOLON))
                Send(KeyCode::END, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & h::Home
            if (key.Is(KeyCode::H))
                Send(KeyCode::HOME, key.state, context, KeyCode::CAPSLOCK);

            // CAPSLOCK & o::Delete
            if (key.Is(KeyCode::O))
                Send(KeyCode::DELETE, key.state, context, KeyCode::CAPSLOCK);

            // often I press CapsLock and Space, after End hotkey (caps + ;),
            // which will keep the CapsLock ON after relese,
            // this will prevent this behaviour
            // CAPSLOCK & SPACE::SPACE
            if (key.Is(KeyCode::SPACE))
                Send(KeyCode::SPACE, key.state, context, KeyCode::CAPSLOCK);
        }

    };
} // namespace HaKey::Layers

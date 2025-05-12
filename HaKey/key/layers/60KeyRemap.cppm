module;

#include <memory>
#include <unordered_set>

export module Layers:_60KeyRemap;

import :BaseHotKeyLayer;
import Core;
import KeyCode;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class _60KeyRemap : public BaseHotKeyLayer
    {
    public:
        void OnKey(Core::KeyContext &context) override
        {
            // update
            HotKeyUpdate(context, KeyCode::CAPSLOCK);
            HotKeyUpdate(context, KeyCode::RIGHTSHIFT);

            if (!context.key.Is(KeyCode::CAPSLOCK) && context.state.IsKeyDown(KeyCode::CAPSLOCK))
                CapsKeySwap(context);
            if (!context.key.Is(KeyCode::RIGHTSHIFT) && context.state.IsKeyDown(KeyCode::RIGHTSHIFT))
                RShiftKeySwap(context);

            // propagate
            next(context);
        }

    private:
        inline void CapsKeySwap(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            // ; for 60% keyboard

            // ; Delete / Shift + Delete
            // CapsLock & g::Delete
            if (key.Is(KeyCode::G))
                Send(KeyCode::DELETE, key.state, context, KeyCode::CAPSLOCK);

            // ; stop build (VS) Ctrl + Break
            // CapsLock & q::^CtrlBreak
            if (key.Is(KeyCode::Q))
            {
                if (key.IsDownOrRepeat())
                {
                    result.AddPressed(KeyCode::LEFTCTRL);
                    result.AddPressed(KeyCode::PAUSE);
                    result.AddReleased(KeyCode::PAUSE);
                    result.AddReleased(KeyCode::LEFTCTRL);
                    context.state.AddGeneratedHotKey(KeyCode::CAPSLOCK);
                }
                context.SuppressKey();
            }

            // ; most programs use F2 for renaming
            // CapsLock & 2::F2
            if (key.Is(KeyCode::N_2))
                Send(KeyCode::F2, key.state, context, KeyCode::CAPSLOCK);

            // ; most programs use F3 to step over find results
            // CapsLock & 3::F3
            if (key.Is(KeyCode::N_3))
                Send(KeyCode::F3, key.state, context, KeyCode::CAPSLOCK);

            // ; F7 and shift F7 is used to switch
            // ; between front-end and back-end files
            // CapsLock & 7::F7
            if (key.Is(KeyCode::N_7))
                Send(KeyCode::F7, key.state, context, KeyCode::CAPSLOCK);
            // ; Alt + F4 => by pressing CapsLock
            // CapsLock & 4::F4
            if (key.Is(KeyCode::N_4))
                Send(KeyCode::F4, key.state, context, KeyCode::CAPSLOCK);

            // ; debugging F5, F9, F10, F11, F12
            // CapsLock & 5::F5
            if (key.Is(KeyCode::N_5))
                Send(KeyCode::F5, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & w::F9
            if (key.Is(KeyCode::W))
                Send(KeyCode::F9, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & e::F10
            if (key.Is(KeyCode::E))
                Send(KeyCode::F10, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & r::F11
            if (key.Is(KeyCode::R))
                Send(KeyCode::F11, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & t::F12
            if (key.Is(KeyCode::T))
                Send(KeyCode::F12, key.state, context, KeyCode::CAPSLOCK);

            // ; no reason for not having F1 and F6-F12 on CapsLock
            // CapsLock & 1::F1
            if (key.Is(KeyCode::N_1))
                Send(KeyCode::F1, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & 6::F6
            if (key.Is(KeyCode::N_6))
                Send(KeyCode::F6, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & 8::F8
            if (key.Is(KeyCode::N_8))
                Send(KeyCode::F8, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & 9::F9
            if (key.Is(KeyCode::N_9))
                Send(KeyCode::F9, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & 0::F10
            if (key.Is(KeyCode::N_0))
                Send(KeyCode::F10, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & -::F11
            if (key.Is(KeyCode::MINUS))
                Send(KeyCode::F11, key.state, context, KeyCode::CAPSLOCK);
            // CapsLock & =::F12
            if (key.Is(KeyCode::EQUAL))
                Send(KeyCode::F12, key.state, context, KeyCode::CAPSLOCK);

            // CapsLock & \::`
            if (key.Is(KeyCode::BACKSLASH))
                Send(KeyCode::GRAVE, key.state, context, KeyCode::CAPSLOCK);
        }

        inline void RShiftKeySwap(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            // ; arrows for right hand, using right shift + pl;'
            // >+p::Send {Up down}
            // >+p Up::Send {Up up}
            if (key.Is(KeyCode::P))
                Send(KeyCode::UP, key.state, context, KeyCode::RIGHTSHIFT);
            // >+l::Send {Left down}
            // >+l Up::Send {Left up}
            if (key.Is(KeyCode::L))
                Send(KeyCode::LEFT, key.state, context, KeyCode::RIGHTSHIFT);
            // >+`;::Send {Down down}
            // >+`; Up::Send {Down up}
            if (key.Is(KeyCode::SEMICOLON))
                Send(KeyCode::DOWN, key.state, context, KeyCode::RIGHTSHIFT);
            // >+'::Send {Right down}
            // >+' Up::Send {Right up}
            if (key.Is(KeyCode::APOSTROPHE))
                Send(KeyCode::RIGHT, key.state, context, KeyCode::RIGHTSHIFT);

            // ; sound control, using right shift + ,./
            // >+,::Send {Volume_Down down}
            // >+, Up::Send {Volume_Down up}
            if (key.Is(KeyCode::COMMA))
                Send(KeyCode::VOLUMEDOWN, key.state, context, KeyCode::RIGHTSHIFT);
            // >+.::Send {Volume_Up down}
            // >+. Up::Send {Volume_Up up}
            if (key.Is(KeyCode::DOT))
                Send(KeyCode::VOLUMEUP, key.state, context, KeyCode::RIGHTSHIFT);
            // >+/::Send {Volume_Mute down}
            // >+/ Up::Send {Volume_Mute up}
            if (key.Is(KeyCode::SLASH))
                Send(KeyCode::MUTE, key.state, context, KeyCode::RIGHTSHIFT);
        }
    };
} // namespace HaKey::Layers

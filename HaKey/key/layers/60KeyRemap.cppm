module;

#include <memory>
#include <unordered_set>

export module Layers:_60KeyRemap;

import Core;
import KeyCode;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class _60KeyRemap : public Core::KeyChainHandler
    {
    private:
        bool _caps_hold = false;
        bool _caps_generated_hotkey = false;
        bool _r_shift_hold = false;
        bool _r_shift_generated_hotkey = false;
        std::unordered_set<KeyCode> active_keys;

    public:
        void OnKey(Core::KeyContext &context) override
        {
            // update
            CapsHotKeyUpdate(context);
            RShiftHotKeyUpdate(context);
            if (!_caps_hold && !_r_shift_hold)
            {
                next(context);
                return;
            }

            if (_caps_hold)
                CapsKeySwap(context);
            if (_r_shift_hold)
                RShiftKeySwap(context);

            // propagate
            next(context);
        }

    private:
        inline void CapsHotKeyUpdate(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            if (key.Is(KeyCode::CAPSLOCK) && key.IsUp())
            {
                if (!_caps_generated_hotkey)
                {
                    result.AddFullKeyOnce(KeyCode::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _caps_generated_hotkey = false;
            }
            else if (key.Is(KeyCode::CAPSLOCK) && key.IsDownOrRepeat())
            {
                _caps_hold = true;
                context.SuppressKey();
            }
        }

        inline void RShiftHotKeyUpdate(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            if (key.Is(KeyCode::RIGHTSHIFT) && key.IsUp())
            {
                if (!_r_shift_generated_hotkey)
                {
                    result.AddFullKeyOnce(KeyCode::RIGHTSHIFT);
                }
                ReleaseActiveKeys(result);
                _r_shift_hold = false;
                _r_shift_generated_hotkey = false;
            }
            else if (key.Is(KeyCode::RIGHTSHIFT) && key.IsDownOrRepeat())
            {
                _r_shift_hold = true;
                context.SuppressKey();
            }
        }

        void ReleaseActiveKeys(Core::KeyResult &result)
        {
            for (KeyCode key : active_keys)
            {
                result.AddKey(key, KeyState::Up);
            }
            active_keys.clear();
        }

        inline void CapsKeySwap(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            // ; for 60% keyboard

            // ; Delete / Shift + Delete
            // CapsLock & g::Delete
            if (key.Is(KeyCode::G))
                CapsSend(KeyCode::DELETE, key.state, context);

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
                    _caps_generated_hotkey = true;
                }
                context.SuppressKey();
            }

            // ; most programs use F2 for renaming
            // CapsLock & 2::F2
            if (key.Is(KeyCode::N_2))
                CapsSend(KeyCode::F2, key.state, context);

            // ; most programs use F3 to step over find results
            // CapsLock & 3::F3
            if (key.Is(KeyCode::N_3))
                CapsSend(KeyCode::F3, key.state, context);

            // ; F7 and shift F7 is used to switch
            // ; between front-end and back-end files
            // CapsLock & 7::F7
            if (key.Is(KeyCode::N_7))
                CapsSend(KeyCode::F7, key.state, context);
            // ; Alt + F4 => by pressing CapsLock
            // CapsLock & 4::F4
            if (key.Is(KeyCode::N_4))
                CapsSend(KeyCode::F4, key.state, context);

            // ; debugging F5, F9, F10, F11, F12
            // CapsLock & 5::F5
            if (key.Is(KeyCode::N_5))
                CapsSend(KeyCode::F5, key.state, context);
            // CapsLock & w::F9
            if (key.Is(KeyCode::W))
                CapsSend(KeyCode::F9, key.state, context);
            // CapsLock & e::F10
            if (key.Is(KeyCode::E))
                CapsSend(KeyCode::F10, key.state, context);
            // CapsLock & r::F11
            if (key.Is(KeyCode::R))
                CapsSend(KeyCode::F11, key.state, context);
            // CapsLock & t::F12
            if (key.Is(KeyCode::T))
                CapsSend(KeyCode::F12, key.state, context);

            // ; no reason for not having F1 and F6-F12 on CapsLock
            // CapsLock & 1::F1
            if (key.Is(KeyCode::N_1))
                CapsSend(KeyCode::F1, key.state, context);
            // CapsLock & 6::F6
            if (key.Is(KeyCode::N_6))
                CapsSend(KeyCode::F6, key.state, context);
            // CapsLock & 8::F8
            if (key.Is(KeyCode::N_8))
                CapsSend(KeyCode::F8, key.state, context);
            // CapsLock & 9::F9
            if (key.Is(KeyCode::N_9))
                CapsSend(KeyCode::F9, key.state, context);
            // CapsLock & 0::F10
            if (key.Is(KeyCode::N_0))
                CapsSend(KeyCode::F10, key.state, context);
            // CapsLock & -::F11
            if (key.Is(KeyCode::MINUS))
                CapsSend(KeyCode::F11, key.state, context);
            // CapsLock & =::F12
            if (key.Is(KeyCode::EQUAL))
                CapsSend(KeyCode::F12, key.state, context);

            // CapsLock & \::`
            if (key.Is(KeyCode::BACKSLASH))
                CapsSend(KeyCode::GRAVE, key.state, context);
        }

        inline void RShiftKeySwap(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            // ; arrows for right hand, using right shift + pl;'
            // >+p::Send {Up down}
            // >+p Up::Send {Up up}
            if (key.Is(KeyCode::P))
                RShiftSend(KeyCode::UP, key.state, context);
            // >+l::Send {Left down}
            // >+l Up::Send {Left up}
            if (key.Is(KeyCode::L))
                RShiftSend(KeyCode::LEFT, key.state, context);
            // >+`;::Send {Down down}
            // >+`; Up::Send {Down up}
            if (key.Is(KeyCode::SEMICOLON))
                RShiftSend(KeyCode::DOWN, key.state, context);
            // >+'::Send {Right down}
            // >+' Up::Send {Right up}
            if (key.Is(KeyCode::APOSTROPHE))
                RShiftSend(KeyCode::RIGHT, key.state, context);

            // ; sound control, using right shift + ,./
            // >+,::Send {Volume_Down down}
            // >+, Up::Send {Volume_Down up}
            if (key.Is(KeyCode::COMMA))
                RShiftSend(KeyCode::VOLUMEDOWN, key.state, context);
            // >+.::Send {Volume_Up down}
            // >+. Up::Send {Volume_Up up}
            if (key.Is(KeyCode::DOT))
                RShiftSend(KeyCode::VOLUMEUP, key.state, context);
            // >+/::Send {Volume_Mute down}
            // >+/ Up::Send {Volume_Mute up}
            if (key.Is(KeyCode::SLASH))
                RShiftSend(KeyCode::MUTE, key.state, context);
        }

        inline void CapsSend(KeyCode key, KeyState state, Core::KeyContext &context)
        {
            context.result.AddKey(key, state);
            _caps_generated_hotkey = true;
            context.SuppressKey();

            if (state == KeyState::Up)
            {
                active_keys.erase(key);
            }
            else if (state == KeyState::Down)
            {
                active_keys.insert(key);
            }
        }

        inline void RShiftSend(KeyCode key, KeyState state, Core::KeyContext &context)
        {
            context.result.AddKey(key, state);
            _r_shift_generated_hotkey = true;
            context.SuppressKey();

            if (state == KeyState::Up)
            {
                active_keys.erase(key);
            }
            else if (state == KeyState::Down)
            {
                active_keys.insert(key);
            }
        }
    };
} // namespace HaKey::Layers

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
        void OnKey(Core::KeyContext& context) override
        {
            // update
            CapsHotKeyUpdate(context.key, context.result);
            RShiftHotKeyUpdate(context.key, context.result);
            if (!_caps_hold && !_r_shift_hold)
            {
                next(context);
                return;
            }

            if (_caps_hold)
                CapsKeySwap(context.key, context.result);
            if (_r_shift_hold)
                RShiftKeySwap(context.key, context.result);

            // propagate
            next(context);
        }

    private:
        inline void CapsHotKeyUpdate(Core::Key key, Core::KeyResult& result)
        {
            if (key.IsUp(KeyCode::CAPSLOCK))
            {
                if (!_caps_generated_hotkey)
                {
                    result.AddFullKey(KeyCode::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _caps_generated_hotkey = false;
            }
            else if (key.IsDownOrRepeat(KeyCode::CAPSLOCK))
            {
                _caps_hold = true;
                result.suppress_original = true;
            }
        }

        inline void RShiftHotKeyUpdate(Core::Key key, Core::KeyResult& result)
        {
            if (key.IsUp(KeyCode::RIGHTSHIFT))
            {
                if (!_r_shift_generated_hotkey)
                {
                    result.AddFullKey(KeyCode::RIGHTSHIFT);
                }
                ReleaseActiveKeys(result);
                _r_shift_hold = false;
                _r_shift_generated_hotkey = false;
            }
            else if (key.IsDownOrRepeat(KeyCode::RIGHTSHIFT))
            {
                _r_shift_hold = true;
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

        inline void CapsKeySwap(Core::Key key, Core::KeyResult& result)
        {
            // ; for 60% keyboard

            // ; Delete / Shift + Delete
            // CapsLock & g::Delete
            if (key.Is(KeyCode::G))
                CapsSend(KeyCode::DELETE, key.state, result);

            // ; stop build (VS) Ctrl + Break
            // CapsLock & q::^CtrlBreak
            if (key.IsDownOrRepeat(KeyCode::Q))
            {
                result.AddPressed(KeyCode::LEFTCTRL);
                result.AddPressed(KeyCode::PAUSE);
                result.AddReleased(KeyCode::PAUSE);
                result.AddReleased(KeyCode::LEFTCTRL);
                _caps_generated_hotkey = true;
                result.suppress_original = true;
            }

            // ; most programs use F2 for renaming
            // CapsLock & 2::F2
            if (key.Is(KeyCode::N_2))
                CapsSend(KeyCode::F2, key.state, result);

            // ; most programs use F3 to step over find results
            // CapsLock & 3::F3
            if (key.Is(KeyCode::N_3))
                CapsSend(KeyCode::F3, key.state, result);

            // ; F7 and shift F7 is used to switch
            // ; between front-end and back-end files
            // CapsLock & 7::F7
            if (key.Is(KeyCode::N_7))
                CapsSend(KeyCode::F7, key.state, result);
            // ; Alt + F4 => by pressing CapsLock
            // CapsLock & 4::F4
            if (key.Is(KeyCode::N_4))
                CapsSend(KeyCode::F4, key.state, result);

            // ; debugging F5, F9, F10, F11, F12
            // CapsLock & 5::F5
            if (key.Is(KeyCode::N_5))
                CapsSend(KeyCode::F5, key.state, result);
            // CapsLock & w::F9
            if (key.Is(KeyCode::W))
                CapsSend(KeyCode::F9, key.state, result);
            // CapsLock & e::F10
            if (key.Is(KeyCode::E))
                CapsSend(KeyCode::F10, key.state, result);
            // CapsLock & r::F11
            if (key.Is(KeyCode::R))
                CapsSend(KeyCode::F11, key.state, result);
            // CapsLock & t::F12
            if (key.Is(KeyCode::T))
                CapsSend(KeyCode::F12, key.state, result);

            // ; no reason for not having F1 and F6-F12 on CapsLock
            // CapsLock & 1::F1
            if (key.Is(KeyCode::N_1))
                CapsSend(KeyCode::F1, key.state, result);
            // CapsLock & 6::F6
            if (key.Is(KeyCode::N_6))
                CapsSend(KeyCode::F6, key.state, result);
            // CapsLock & 8::F8
            if (key.Is(KeyCode::N_8))
                CapsSend(KeyCode::F8, key.state, result);
            // CapsLock & 9::F9
            if (key.Is(KeyCode::N_9))
                CapsSend(KeyCode::F9, key.state, result);
            // CapsLock & 0::F10
            if (key.Is(KeyCode::N_0))
                CapsSend(KeyCode::F10, key.state, result);
            // CapsLock & -::F11
            if (key.Is(KeyCode::MINUS))
                CapsSend(KeyCode::F11, key.state, result);
            // CapsLock & =::F12
            if (key.Is(KeyCode::EQUAL))
                CapsSend(KeyCode::F12, key.state, result);

            // CapsLock & \::`
            if (key.Is(KeyCode::BACKSLASH))
                CapsSend(KeyCode::GRAVE, key.state, result);
        }

        inline void RShiftKeySwap(Core::Key key, Core::KeyResult& result)
        {
            // ; arrows for right hand, using right shift + pl;'
            // >+p::Send {Up down}
            // >+p Up::Send {Up up}
            if (key.Is(KeyCode::P))
                RShiftSend(KeyCode::UP, key.state, result);
            // >+l::Send {Left down}
            // >+l Up::Send {Left up}
            if (key.Is(KeyCode::L))
                RShiftSend(KeyCode::LEFT, key.state, result);
            // >+`;::Send {Down down}
            // >+`; Up::Send {Down up}
            if (key.Is(KeyCode::SEMICOLON))
                RShiftSend(KeyCode::DOWN, key.state, result);
            // >+'::Send {Right down}
            // >+' Up::Send {Right up}
            if (key.Is(KeyCode::APOSTROPHE))
                RShiftSend(KeyCode::RIGHT, key.state, result);

            // ; sound control, using right shift + ,./
            // >+,::Send {Volume_Down down}
            // >+, Up::Send {Volume_Down up}
            if (key.Is(KeyCode::COMMA))
                RShiftSend(KeyCode::VOLUMEDOWN, key.state, result);
            // >+.::Send {Volume_Up down}
            // >+. Up::Send {Volume_Up up}
            if (key.Is(KeyCode::DOT))
                RShiftSend(KeyCode::VOLUMEUP, key.state, result);
            // >+/::Send {Volume_Mute down}
            // >+/ Up::Send {Volume_Mute up}
            if (key.Is(KeyCode::SLASH))
                RShiftSend(KeyCode::MUTE, key.state, result);
        }

        inline void CapsSend(KeyCode key, KeyState state, Core::KeyResult& result)
        {
            result.AddKey(key, state);
            _caps_generated_hotkey = true;
            result.suppress_original = true;

            if (state == KeyState::Up)
            {
                active_keys.erase(key);
            }
            else if (state == KeyState::Down)
            {
                active_keys.insert(key);
            }
        }

        inline void RShiftSend(KeyCode key, KeyState state, Core::KeyResult& result)
        {
            result.AddKey(key, state);
            _r_shift_generated_hotkey = true;
            result.suppress_original = true;

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

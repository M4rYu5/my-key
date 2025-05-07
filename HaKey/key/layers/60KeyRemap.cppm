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
        void OnKey(Core::Key key, std::shared_ptr<Core::KeyResult> result) override
        {
            // update
            CapsHotKeyUpdate(key, result);
            RShiftHotKeyUpdate(key, result);
            if (!_caps_hold && !_r_shift_hold)
            {
                next(key, result);
                return;
            }

            if (_caps_hold)
                CapsKeySwap(key, result);
            if (_r_shift_hold)
                RShiftKeySwap(key, result);

            // propagate
            next(key, result);
        }

    private:
        inline void CapsHotKeyUpdate(Core::Key key, std::shared_ptr<Core::KeyResult> result)
        {
            if (IsKeyUp(KeyCode::CAPSLOCK, key))
            {
                if (!_caps_generated_hotkey)
                {
                    result->AddFullKey(KeyCode::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _caps_generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(KeyCode::CAPSLOCK, key))
            {
                _caps_hold = true;
                result->suppress_original = true;
            }
        }

        inline void RShiftHotKeyUpdate(Core::Key key, std::shared_ptr<Core::KeyResult> result)
        {
            if (IsKeyUp(KeyCode::RIGHTSHIFT, key))
            {
                if (!_r_shift_generated_hotkey)
                {
                    result->AddFullKey(KeyCode::RIGHTSHIFT);
                }
                ReleaseActiveKeys(result);
                _r_shift_hold = false;
                _r_shift_generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(KeyCode::RIGHTSHIFT, key))
            {
                _r_shift_hold = true;
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

        inline void CapsKeySwap(Core::Key key, std::shared_ptr<Core::KeyResult> result)
        {
            // ; for 60% keyboard

            // ; Delete / Shift + Delete
            // CapsLock & g::Delete
            if (IsKey(KeyCode::G, key))
                CapsSend(KeyCode::DELETE, key.state, result);

            // ; stop build (VS) Ctrl + Break
            // CapsLock & q::^CtrlBreak
            if (IsKeyDownOrRepeat(KeyCode::Q, key))
            {
                result->AddPressed(KeyCode::LEFTCTRL);
                result->AddPressed(KeyCode::PAUSE);
                result->AddReleased(KeyCode::PAUSE);
                result->AddReleased(KeyCode::LEFTCTRL);
                _caps_generated_hotkey = true;
                result->suppress_original = true;
            }

            // ; most programs use F2 for renaming
            // CapsLock & 2::F2
            if (IsKey(KeyCode::N_2, key))
                CapsSend(KeyCode::F2, key.state, result);

            // ; most programs use F3 to step over find results
            // CapsLock & 3::F3
            if (IsKey(KeyCode::N_3, key))
                CapsSend(KeyCode::F3, key.state, result);

            // ; F7 and shift F7 is used to switch
            // ; between front-end and back-end files
            // CapsLock & 7::F7
            if (IsKey(KeyCode::N_7, key))
                CapsSend(KeyCode::F7, key.state, result);
            // ; Alt + F4 => by pressing CapsLock
            // CapsLock & 4::F4
            if (IsKey(KeyCode::N_4, key))
                CapsSend(KeyCode::F4, key.state, result);

            // ; debugging F5, F9, F10, F11, F12
            // CapsLock & 5::F5
            if (IsKey(KeyCode::N_5, key))
                CapsSend(KeyCode::F5, key.state, result);
            // CapsLock & w::F9
            if (IsKey(KeyCode::W, key))
                CapsSend(KeyCode::F9, key.state, result);
            // CapsLock & e::F10
            if (IsKey(KeyCode::E, key))
                CapsSend(KeyCode::F10, key.state, result);
            // CapsLock & r::F11
            if (IsKey(KeyCode::R, key))
                CapsSend(KeyCode::F11, key.state, result);
            // CapsLock & t::F12
            if (IsKey(KeyCode::T, key))
                CapsSend(KeyCode::F12, key.state, result);

            // ; no reason for not having F1 and F6-F12 on CapsLock
            // CapsLock & 1::F1
            if (IsKey(KeyCode::N_1, key))
                CapsSend(KeyCode::F1, key.state, result);
            // CapsLock & 6::F6
            if (IsKey(KeyCode::N_6, key))
                CapsSend(KeyCode::F6, key.state, result);
            // CapsLock & 8::F8
            if (IsKey(KeyCode::N_8, key))
                CapsSend(KeyCode::F8, key.state, result);
            // CapsLock & 9::F9
            if (IsKey(KeyCode::N_9, key))
                CapsSend(KeyCode::F9, key.state, result);
            // CapsLock & 0::F10
            if (IsKey(KeyCode::N_0, key))
                CapsSend(KeyCode::F10, key.state, result);
            // CapsLock & -::F11
            if (IsKey(KeyCode::MINUS, key))
                CapsSend(KeyCode::F11, key.state, result);
            // CapsLock & =::F12
            if (IsKey(KeyCode::EQUAL, key))
                CapsSend(KeyCode::F12, key.state, result);

            // CapsLock & \::`
            if (IsKey(KeyCode::BACKSLASH, key))
                CapsSend(KeyCode::GRAVE, key.state, result);
        }

        inline void RShiftKeySwap(Core::Key key, std::shared_ptr<Core::KeyResult> result)
        {
            // ; arrows for right hand, using right shift + pl;'
            // >+p::Send {Up down}
            // >+p Up::Send {Up up}
            if (IsKey(KeyCode::P, key))
                RShiftSend(KeyCode::UP, key.state, result);
            // >+l::Send {Left down}
            // >+l Up::Send {Left up}
            if (IsKey(KeyCode::L, key))
                RShiftSend(KeyCode::LEFT, key.state, result);
            // >+`;::Send {Down down}
            // >+`; Up::Send {Down up}
            if (IsKey(KeyCode::SEMICOLON, key))
                RShiftSend(KeyCode::DOWN, key.state, result);
            // >+'::Send {Right down}
            // >+' Up::Send {Right up}
            if (IsKey(KeyCode::APOSTROPHE, key))
                RShiftSend(KeyCode::RIGHT, key.state, result);

            // ; sound control, using right shift + ,./
            // >+,::Send {Volume_Down down}
            // >+, Up::Send {Volume_Down up}
            if (IsKey(KeyCode::COMMA, key))
                RShiftSend(KeyCode::VOLUMEDOWN, key.state, result);
            // >+.::Send {Volume_Up down}
            // >+. Up::Send {Volume_Up up}
            if (IsKey(KeyCode::DOT, key))
                RShiftSend(KeyCode::VOLUMEUP, key.state, result);
            // >+/::Send {Volume_Mute down}
            // >+/ Up::Send {Volume_Mute up}
            if (IsKey(KeyCode::SLASH, key))
                RShiftSend(KeyCode::MUTE, key.state, result);
        }

        inline void CapsSend(KeyCode key, KeyState state, std::shared_ptr<Core::KeyResult> result)
        {
            result->AddKey(key, state);
            _caps_generated_hotkey = true;
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

        inline void RShiftSend(KeyCode key, KeyState state, std::shared_ptr<Core::KeyResult> result)
        {
            result->AddKey(key, state);
            _r_shift_generated_hotkey = true;
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

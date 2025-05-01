module;

#include <memory>

export module Layers:_60KeyRemap;

import Core;
import VKey;
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
        std::unordered_set<VKey> active_keys;

    public:
        void OnKey(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result) override
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
        inline void CapsHotKeyUpdate(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            if (IsKeyUp(VKey::CAPSLOCK, key))
            {
                if (!_caps_generated_hotkey)
                {
                    result->AddFullKey(VKey::CAPSLOCK);
                }
                ReleaseActiveKeys(result);
                _caps_hold = false;
                _caps_generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(VKey::CAPSLOCK, key))
            {
                _caps_hold = true;
                result->suppress_original = true;
            }
        }

        inline void RShiftHotKeyUpdate(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            if (IsKeyUp(VKey::RIGHTSHIFT, key))
            {
                if (!_r_shift_generated_hotkey)
                {
                    result->AddFullKey(VKey::RIGHTSHIFT);
                }
                ReleaseActiveKeys(result);
                _r_shift_hold = false;
                _r_shift_generated_hotkey = false;
            }
            else if (IsKeyDownOrRepeat(VKey::RIGHTSHIFT, key))
            {
                _r_shift_hold = true;
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

        inline void CapsKeySwap(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            // ; for 60% keyboard

            // ; Delete / Shift + Delete
            // CapsLock & g::Delete
            if (IsKey(VKey::G, key))
                CapsSend(VKey::DELETE, key.state, result);

            // ; stop build (VS) Ctrl + Break
            // CapsLock & q::^CtrlBreak
            if (IsKeyDownOrRepeat(VKey::Q, key))
            {
                result->AddPressed(VKey::LEFTCTRL);
                result->AddPressed(VKey::PAUSE);
                result->AddReleased(VKey::PAUSE);
                result->AddReleased(VKey::LEFTCTRL);
                _caps_generated_hotkey = true;
                result->suppress_original = true;
            }

            // ; most programs use F2 for renaming
            // CapsLock & 2::F2
            if (IsKey(VKey::N_2, key))
                CapsSend(VKey::F2, key.state, result);

            // ; most programs use F3 to step over find results
            // CapsLock & 3::F3
            if (IsKey(VKey::N_3, key))
                CapsSend(VKey::F3, key.state, result);

            // ; F7 and shift F7 is used to switch
            // ; between front-end and back-end files
            // CapsLock & 7::F7
            if (IsKey(VKey::N_7, key))
                CapsSend(VKey::F7, key.state, result);
            // ; Alt + F4 => by pressing CapsLock
            // CapsLock & 4::F4
            if (IsKey(VKey::N_4, key))
                CapsSend(VKey::F4, key.state, result);

            // ; debugging F5, F9, F10, F11, F12
            // CapsLock & 5::F5
            if (IsKey(VKey::N_5, key))
                CapsSend(VKey::F5, key.state, result);
            // CapsLock & w::F9
            if (IsKey(VKey::W, key))
                CapsSend(VKey::F9, key.state, result);
            // CapsLock & e::F10
            if (IsKey(VKey::E, key))
                CapsSend(VKey::F10, key.state, result);
            // CapsLock & r::F11
            if (IsKey(VKey::R, key))
                CapsSend(VKey::F11, key.state, result);
            // CapsLock & t::F12
            if (IsKey(VKey::T, key))
                CapsSend(VKey::F12, key.state, result);

            // ; no reason for not having F1 and F6-F12 on CapsLock
            // CapsLock & 1::F1
            if (IsKey(VKey::N_1, key))
                CapsSend(VKey::F1, key.state, result);
            // CapsLock & 6::F6
            if (IsKey(VKey::N_6, key))
                CapsSend(VKey::F6, key.state, result);
            // CapsLock & 8::F8
            if (IsKey(VKey::N_8, key))
                CapsSend(VKey::F8, key.state, result);
            // CapsLock & 9::F9
            if (IsKey(VKey::N_9, key))
                CapsSend(VKey::F9, key.state, result);
            // CapsLock & 0::F10
            if (IsKey(VKey::N_0, key))
                CapsSend(VKey::F10, key.state, result);
            // CapsLock & -::F11
            if (IsKey(VKey::MINUS, key))
                CapsSend(VKey::F11, key.state, result);
            // CapsLock & =::F12
            if (IsKey(VKey::EQUAL, key))
                CapsSend(VKey::F12, key.state, result);

            // CapsLock & \::`
            if (IsKey(VKey::BACKSLASH, key))
                CapsSend(VKey::GRAVE, key.state, result);
        }

        inline void RShiftKeySwap(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result)
        {
            // ; arrows for right hand, using right shift + pl;'
            // >+p::Send {Up down}
            // >+p Up::Send {Up up}
            if (IsKey(VKey::P, key))
                RShiftSend(VKey::UP, key.state, result);
            // >+l::Send {Left down}
            // >+l Up::Send {Left up}
            if (IsKey(VKey::L, key))
                RShiftSend(VKey::LEFT, key.state, result);
            // >+`;::Send {Down down}
            // >+`; Up::Send {Down up}
            if (IsKey(VKey::SEMICOLON, key))
                RShiftSend(VKey::DOWN, key.state, result);
            // >+'::Send {Right down}
            // >+' Up::Send {Right up}
            if (IsKey(VKey::APOSTROPHE, key))
                RShiftSend(VKey::RIGHT, key.state, result);

            // ; sound control, using right shift + ,./
            // >+,::Send {Volume_Down down}
            // >+, Up::Send {Volume_Down up}
            if (IsKey(VKey::COMMA, key))
                RShiftSend(VKey::VOLUMEDOWN, key.state, result);
            // >+.::Send {Volume_Up down}
            // >+. Up::Send {Volume_Up up}
            if (IsKey(VKey::DOT, key))
                RShiftSend(VKey::VOLUMEUP, key.state, result);
            // >+/::Send {Volume_Mute down}
            // >+/ Up::Send {Volume_Mute up}
            if (IsKey(VKey::SLASH, key))
                RShiftSend(VKey::MUTE, key.state, result);
        }

        inline void CapsSend(VKey key, KeyState state, std::shared_ptr<Core::KeyResult> result)
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

        inline void RShiftSend(VKey key, KeyState state, std::shared_ptr<Core::KeyResult> result)
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

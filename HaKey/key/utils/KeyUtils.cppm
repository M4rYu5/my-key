module;

export module KeyUtils;

import Core;
import KeyCode;

namespace HaKey::Utils
{
    namespace Core = HaKey::Core;

    export char KeyCodeToChar(KeyCode kc)
    {
        // This function returns the non-shifted character for a given KeyCode.
        // It's used to build the buffer for string matching.
        switch (kc)
        {
        case KeyCode::A: return 'a'; case KeyCode::B: return 'b'; case KeyCode::C: return 'c';
        case KeyCode::D: return 'd'; case KeyCode::E: return 'e'; case KeyCode::F: return 'f';
        case KeyCode::G: return 'g'; case KeyCode::H: return 'h'; case KeyCode::I: return 'i';
        case KeyCode::J: return 'j'; case KeyCode::K: return 'k'; case KeyCode::L: return 'l';
        case KeyCode::M: return 'm'; case KeyCode::N: return 'n'; case KeyCode::O: return 'o';
        case KeyCode::P: return 'p'; case KeyCode::Q: return 'q'; case KeyCode::R: return 'r';
        case KeyCode::S: return 's'; case KeyCode::T: return 't'; case KeyCode::U: return 'u';
        case KeyCode::V: return 'v'; case KeyCode::W: return 'w'; case KeyCode::X: return 'x';
        case KeyCode::Y: return 'y'; case KeyCode::Z: return 'z';

        case KeyCode::N_0: return '0'; case KeyCode::N_1: return '1'; case KeyCode::N_2: return '2';
        case KeyCode::N_3: return '3'; case KeyCode::N_4: return '4'; case KeyCode::N_5: return '5';
        case KeyCode::N_6: return '6'; case KeyCode::N_7: return '7'; case KeyCode::N_8: return '8';
        case KeyCode::N_9: return '9';

        case KeyCode::KP_0: return '0'; case KeyCode::KP_1: return '1'; case KeyCode::KP_2: return '2';
        case KeyCode::KP_3: return '3'; case KeyCode::KP_4: return '4'; case KeyCode::KP_5: return '5';
        case KeyCode::KP_6: return '6'; case KeyCode::KP_7: return '7'; case KeyCode::KP_8: return '8';
        case KeyCode::KP_9: return '9';

        case KeyCode::SPACE: return ' ';
        case KeyCode::MINUS: return '-';
        case KeyCode::EQUAL: return '=';
        case KeyCode::LEFTBRACE: return '[';
        case KeyCode::RIGHTBRACE: return ']';
        case KeyCode::BACKSLASH: return '\\';
        case KeyCode::SEMICOLON: return ';';
        case KeyCode::APOSTROPHE: return '\'';
        case KeyCode::GRAVE: return '`';
        case KeyCode::COMMA: return ',';
        case KeyCode::DOT: return '.';
        case KeyCode::SLASH: return '/';
        case KeyCode::KP_SLASH: return '/';
        case KeyCode::KP_ASTERISK: return '*';
        case KeyCode::KP_MINUS: return '-';
        case KeyCode::KP_PLUS: return '+';
        case KeyCode::KP_DOT: return '.';

        default: return '\0';
        }
    }

    export void SendChar(char c, Core::KeyResult &result)
    {
        bool shift = false;
        KeyCode kc = KeyCode::NONE;

        if (c >= 'a' && c <= 'z')
        {
            kc = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (c - 'a'));
        }
        else if (c >= 'A' && c <= 'Z')
        {
            shift = true;
            kc = static_cast<KeyCode>(static_cast<int>(KeyCode::A) + (c - 'A'));
        }
        else
        {
            // US keyboard layout assumed for symbols
            switch (c)
            {
            case ' ': kc = KeyCode::SPACE; break;
            case '`': kc = KeyCode::GRAVE; break;
            case '~': shift = true; kc = KeyCode::GRAVE; break;
            case '1': kc = KeyCode::N_1; break;
            case '!': shift = true; kc = KeyCode::N_1; break;
            case '2': kc = KeyCode::N_2; break;
            case '@': shift = true; kc = KeyCode::N_2; break;
            case '3': kc = KeyCode::N_3; break;
            case '#': shift = true; kc = KeyCode::N_3; break;
            case '4': kc = KeyCode::N_4; break;
            case '$': shift = true; kc = KeyCode::N_4; break;
            case '5': kc = KeyCode::N_5; break;
            case '%': shift = true; kc = KeyCode::N_5; break;
            case '6': kc = KeyCode::N_6; break;
            case '^': shift = true; kc = KeyCode::N_6; break;
            case '7': kc = KeyCode::N_7; break;
            case '&': shift = true; kc = KeyCode::N_7; break;
            case '8': kc = KeyCode::N_8; break;
            case '*': shift = true; kc = KeyCode::N_8; break;
            case '9': kc = KeyCode::N_9; break;
            case '(': shift = true; kc = KeyCode::N_9; break;
            case '0': kc = KeyCode::N_0; break;
            case ')': shift = true; kc = KeyCode::N_0; break;
            case '-': kc = KeyCode::MINUS; break;
            case '_': shift = true; kc = KeyCode::MINUS; break;
            case '=': kc = KeyCode::EQUAL; break;
            case '+': shift = true; kc = KeyCode::EQUAL; break;
            case '[': kc = KeyCode::LEFTBRACE; break; case '{': shift = true; kc = KeyCode::LEFTBRACE; break;
            case ']': kc = KeyCode::RIGHTBRACE; break; case '}': shift = true; kc = KeyCode::RIGHTBRACE; break;
            case '\\': kc = KeyCode::BACKSLASH; break; case '|': shift = true; kc = KeyCode::BACKSLASH; break;
            case ';': kc = KeyCode::SEMICOLON; break; case ':': shift = true; kc = KeyCode::SEMICOLON; break;
            case '\'': kc = KeyCode::APOSTROPHE; break; case '"': shift = true; kc = KeyCode::APOSTROPHE; break;
            case ',': kc = KeyCode::COMMA; break; case '<': shift = true; kc = KeyCode::COMMA; break;
            case '.': kc = KeyCode::DOT; break; case '>': shift = true; kc = KeyCode::DOT; break;
            case '/': kc = KeyCode::SLASH; break; case '?': shift = true; kc = KeyCode::SLASH; break;
            }
        }

        if (kc == KeyCode::NONE) return;

        if (shift) result.AddPressed(KeyCode::LEFTSHIFT);
        result.AddFullKey(kc);
        if (shift) result.AddReleased(KeyCode::LEFTSHIFT);
    }
}

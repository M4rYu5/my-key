module;

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

export module Layers:BaseStringLayer;

import Core;
import KeyCode;
import KeyState;
import KeyUtils;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;
    namespace Utils = HaKey::Utils;

    export class BaseStringLayer : public Core::KeyChainHandler
    {
    private:
        std::deque<KeyCode> key_buffer;
        size_t max_buffer_size = 0;

    protected:
        void SetMaxBufferSize(size_t size)
        {
            max_buffer_size = size;
        }

        void ProcessKey(Core::KeyContext &context, const std::unordered_map<std::string, std::string> &replacements)
        {
            Core::Key &key = context.key;

            if (key.IsUp())
            {
                char typed_char = Utils::KeyCodeToChar(key.code);
                if (typed_char != '\0')
                {
                    key_buffer.push_back(key.code);
                    if (key_buffer.size() > max_buffer_size)
                    {
                        key_buffer.pop_front();
                    }

                    std::string current_buffer_str;
                    for (const auto &kc : key_buffer)
                    {
                        current_buffer_str += Utils::KeyCodeToChar(kc);
                    }

                    for (const auto &pair : replacements)
                    {
                        if (current_buffer_str.ends_with(pair.first))
                        {
                            // Match found, perform replacement
                            for (size_t i = 0; i < pair.first.length(); ++i)
                            {
                                context.result.AddFullKey(KeyCode::BACKSPACE);
                            }

                            for (char c : pair.second)
                            {
                                Utils::SendChar(c, context.result);
                            }

                            key_buffer.clear();
                            context.SuppressKey(); // Suppress the original key to prevent it from being typed
                            return;
                        }
                    }
                }
                else
                {
                    // non-alphanumeric key pressed, clear buffer
                    key_buffer.clear();
                }
            }
        }

    public:
        virtual void OnKey(Core::KeyContext &context) override = 0;
    };
} // namespace HaKey::Layers
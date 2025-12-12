module;

#include <deque>
#include <functional>
#include <optional>
#include <string>
#include <tuple>
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
        std::optional<std::tuple<KeyCode, std::string, std::function<std::string()>>> pending_replacement;
        std::deque<KeyCode> key_buffer;
        size_t max_buffer_size = 0;

    protected:
        std::unordered_map<std::string, std::function<std::string()>> replacements;

        void UpdateMaxBufferSize()
        {
            size_t size = 0;
            for (const auto &pair : replacements)
            {
                size = std::max(size, pair.first.length());
            }
            max_buffer_size = size;
        }

    protected:
        void ProcessKey(Core::KeyContext &context)
        {
            Core::Key &key = context.key;
            
            if (key.IsDown())
            {
                if (Utils::KeyCodeToChar(key.code) != '\0')
                {
                    HandleAlphanumericKey(context);
                }
                else
                {
                    key_buffer.clear();
                }
            }
            else if (key.IsUp() && pending_replacement.has_value())
            {
                if (std::get<0>(*pending_replacement) == key.code)
                {
                    PerformReplacement(context, std::get<1>(*pending_replacement), std::get<2>(*pending_replacement)());
                    pending_replacement.reset();
                }
            }
        }

    private:
        void HandleAlphanumericKey(Core::KeyContext &context)
        {
            UpdateKeyBuffer(context.key.code);
            CheckForAndSaveReplacement(context);
        }

        void UpdateKeyBuffer(KeyCode kc)
        {
            key_buffer.push_back(kc);
            if (key_buffer.size() > max_buffer_size)
            {
                key_buffer.pop_front();
            }
        }

        void CheckForAndSaveReplacement(Core::KeyContext &context)
        {
            std::string current_buffer_str;
            for (const auto &kc : key_buffer)
            {
                current_buffer_str += Utils::KeyCodeToChar(kc);
            }

            for (const auto &pair : replacements)
            {
                if (current_buffer_str.ends_with(pair.first))
                {
                    pending_replacement = std::make_tuple(context.key.code, pair.first, pair.second);
                    return;
                }
            }
        }

        void PerformReplacement(Core::KeyContext &context, const std::string &trigger, const std::string &replacement_text)
        {
            for (size_t i = 0; i < trigger.length(); ++i)
            {
                context.result.AddFullKey(KeyCode::BACKSPACE);
            }

            for (char c : replacement_text)
            {
                Utils::SendChar(c, context.result);
            }

            key_buffer.clear();
        }

    public:
        virtual ~BaseStringLayer() = default;

        virtual void OnKey(Core::KeyContext &context) override = 0;
    };
} // namespace HaKey::Layers
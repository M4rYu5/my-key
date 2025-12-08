module;

#include <algorithm>
#include <chrono>
#include <format>
#include <functional>
#include <string>
#include <unordered_map>

export module Layers:StringReplaceLayer;

import :BaseStringLayer;
import Core;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class StringReplaceLayer : public BaseStringLayer
    {
    private:
        std::unordered_map<std::string, std::function<std::string()>> replacements;

    public:
        StringReplaceLayer()
        {
            replacements = {
                {"4vid", []() { return "document.getElementsByTagName('video')[0].playbackRate = 3"; }},
                {"4date",
                 []()
                 {
                     const auto now = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now());
                     return std::format("{:%Y-%m-%d}", now);
                 }},
                {"4time",
                 []()
                 {
                     const auto now = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now());
                     return std::format("{:%H:%M}", now);
                 }},
                {"4dt",
                 []()
                 {
                     const auto now = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now());
                     return std::format("{:%Y-%m-%d %H:%M}", now);
                 }},

            };

            size_t max_len = 0;
            for (const auto &pair : replacements)
            {
                max_len = std::max(max_len, pair.first.length());
            }
            SetMaxBufferSize(max_len);
        }

        void OnKey(Core::KeyContext &context) override
        {
            ProcessKey(context, replacements);

            next(context);
        }
    };
} // namespace HaKey::Layers
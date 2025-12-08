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
    public:
        StringReplaceLayer()
        {
            InitializeReplacements();
            UpdateMaxBufferSize();
        }
    protected:
        void InitializeReplacements()
        {
            replacements = {
                {"4vid", []()
                 { return "document.getElementsByTagName('video')[0].playbackRate = 3"; }},
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
                 }}
            };
        }

    public:
        void OnKey(Core::KeyContext &context) override
        {
            ProcessKey(context);
            next(context);
        }
    };
} // namespace HaKey::Layers
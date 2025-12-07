module;

#include <string>
#include <unordered_map>
#include <algorithm>

export module Layers:StringReplaceLayer;

import :BaseStringLayer;
import Core;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

    export class StringReplaceLayer : public BaseStringLayer
    {
    private:
        std::unordered_map<std::string, std::string> replacements;

    public:
        StringReplaceLayer()
        {
            replacements = {
                {"4str", "Yes string replacement is working!"}};

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
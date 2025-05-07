module;

#include <memory>
#include <unordered_map>

export module Core:KeyContext;

import :Key;
import :KeyResult;


namespace HaKey::Core {

	export class KeyContext{
        public:
            Key key;
            KeyResult result;

            inline void Reset(const Key& newKey){
                key = newKey;
                result.Clear();
            }
    };

}
module;

#include <memory>
#include <unordered_map>

export module Core:KeyContext;

import :Key;
import :KeyResult;
import :KeyShare;


namespace HaKey::Core {

	export class KeyContext{
        private:
            bool suppress_key = false;
        public:
            Key key;
            KeyResult result;
            /// @brief Contains shared states of different type of keys.
            KeyShare state;


            inline void SuppressKey(){
                suppress_key = true;
            }

            inline bool IsKeySuppressed(){
                return suppress_key;
            }

            inline void Reset(const Key& newKey){
                key = newKey;
                suppress_key = false;
                result.Clear();
            }

    };

}
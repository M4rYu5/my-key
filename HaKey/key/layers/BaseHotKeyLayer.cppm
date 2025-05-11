module;

#include <memory>
#include <unordered_set>

export module Layers:BaseHotKeyLayer;

import Core;
import KeyCode;
import KeyState;

namespace HaKey::Layers
{
    namespace Core = HaKey::Core;

	export class BaseHotKeyLayer : public Core::KeyChainHandler {
    private:
        std::unordered_set<KeyCode> active_keys;

	public:
		/// <summary>
		/// Handle the key event. You have to call next(context), for key propagation.
		/// </summary>
		virtual void OnKey(Core::KeyContext& context) override = 0;

		virtual ~BaseHotKeyLayer() override = default;

    protected:
        void HotKeyUpdate(Core::KeyContext& context, KeyCode hot_key_code)
        {
            Core::Key &key = context.key;
            Core::KeyResult &result = context.result;

            if (key.Is(hot_key_code) && key.IsUp())
            {
                if (!context.state.GeneratedHotKey(hot_key_code))
                {
                    result.AddFullKeyOnce(hot_key_code);
                }
                ReleaseActiveKeys(result);
            }
            else if (key.Is(hot_key_code) && key.IsDownOrRepeat())
            {
                context.SuppressKey();
            }
        }

        void Send(KeyCode code, KeyState state, Core::KeyContext& context, KeyCode hot_key_code)
        {
            context.result.AddKey(code, state);
            context.state.AddGeneratedHotKey(hot_key_code);
            context.SuppressKey();

            if (state == KeyState::Up)
            {
                active_keys.erase(code);
            }
            else if (state == KeyState::Down)
            {
                active_keys.insert(code);
            }
        }

    private:
        void ReleaseActiveKeys(Core::KeyResult& result)
        {
            for (KeyCode key : active_keys)
            {
                result.AddKey(key, KeyState::Up);
            }
            active_keys.clear();
        }

	};
}
module;

#include <memory>

export module Core:KeyChainHandler;

import :Key;
import :KeyResult;
import :KeyContext;

namespace HaKey::Core {

	export class KeyChainHandler {
	private:
		std::unique_ptr<KeyChainHandler> _next = nullptr;
	protected:
		void next(KeyContext& context){
			if (_next){
				_next->OnKey(context);
			}
		}
	public:
		/// <summary>
		/// Handle the key event. You have to call next(context), for key propagation.
		/// </summary>
		virtual void OnKey(KeyContext& context) = 0;

		/// <summary>
		/// Add a new handler
		/// </summary>
		/// <param name="k">handler</param>
		void Add(std::unique_ptr<KeyChainHandler> k) {
			if (_next) {
				_next->Add(std::move(k));
			}
			else {
				_next = std::move(k);
			}
		}

		virtual ~KeyChainHandler() = default;
	};
}
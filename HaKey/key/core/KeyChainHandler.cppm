module;

#include <memory>

export module Core:KeyChainHandler;

export import :KeyState;
export import :KeyEvent;
export import :KeyResult;

namespace HaKey::Core {

	export class KeyChainHandler {
	protected:
		std::unique_ptr<KeyChainHandler> next = nullptr;
	public:
		/// <summary>
		/// Handle the key event. You have to call next->OnKey, for key propagation.
		/// </summary>
		virtual void OnKey(KeyEvent key, std::shared_ptr<KeyResult> result) = 0;

		/// <summary>
		/// Add a new handler
		/// </summary>
		/// <param name="k">handler</param>
		void Add(std::unique_ptr<KeyChainHandler> k) {
			if (next) {
				next->Add(std::move(k));
			}
			else {
				next = std::move(k);
			}
		}

		virtual ~KeyChainHandler() = default;
	};
}
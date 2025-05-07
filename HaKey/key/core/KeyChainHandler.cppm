module;

#include <memory>

export module Core:KeyChainHandler;

export import :Key;
export import :KeyResult;

namespace HaKey::Core {

	export class KeyChainHandler {
	private:
		std::unique_ptr<KeyChainHandler> _next = nullptr;
	protected:
		void next(Key key, std::shared_ptr<KeyResult> result){
			if (_next){
				_next->OnKey(key, result);
			}
		}
	public:
		/// <summary>
		/// Handle the key event. You have to call next(key, result), for key propagation.
		/// </summary>
		virtual void OnKey(Key key, std::shared_ptr<KeyResult> result) = 0;

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
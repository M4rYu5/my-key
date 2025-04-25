module;

#include <memory>

export module Key:Dispatcher;

import :ChainHandler;

namespace Key {

	export class KeyDispatcher : public Core::KeyChainHandler {

	private:
		void OnKey(std::shared_ptr<Core::KeyEvent> key, std::shared_ptr<Core::KeyResult> result) override {
			next->OnKey(key, result);
		}


	};

}
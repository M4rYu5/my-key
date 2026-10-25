module;

#include <memory>


export module KeyDispatcher;

import Core;
import System;
import Layers;

namespace HaKey
{

	export class KeyDispatcher : public Core::KeyChainHandler
	{
	private:
		std::unique_ptr<System::ISystemKeyDispatcher> _dispatcher = nullptr;
		Core::KeyContext context = Core::KeyContext();

	public:
		void Listen()
		{
			// propagate keys that are pressed in combination with HotKeys, keeping modifiers
			Add(std::make_unique<Layers::PropagateKey>());

			// init system dispatcher
#if LINUX
			_dispatcher = std::make_unique<System::LinuxKeyDispatcher>(
				[this](Core::Key key) { this->KeyHandler(key); },
				[this]() { Reset(); });
			_dispatcher->Listen();
#endif
#if WINDOWS
			_dispatcher = std::make_unique<System::WindowsKeyDispatcher>([this](Core::Key key) { this->KeyHandler(key); });
			_dispatcher->Listen();
#endif
		}

		void Reset() override
		{
			context = Core::KeyContext();
			Core::KeyChainHandler::Reset();
		}

	private:
		void KeyHandler(Core::Key key)
		{
			context.Reset(key);
			context.state.OnEarlyKeyEvent(key);

			OnKey(context);

			_dispatcher->Send(context.result.keys);
			context.state.OnLateKeyEvent(key);
		}

		void OnKey(Core::KeyContext &context) override
		{
			next(context);
		}

	};
} // HaKey namespace
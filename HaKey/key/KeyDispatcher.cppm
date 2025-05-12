module;

#include <memory>
#include <iostream>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>

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
		void Listen(int linux_device_id = 0)
		{
			// propagate keys that are pressed in combination with HotKeys, keeping modifiers
			std::unique_ptr<Core::KeyChainHandler> propagate = std::make_unique<Layers::PropagateKey>();
			Add(std::move(propagate));

			// init system dispatcher
			_dispatcher = std::make_unique<System::LinuxKeyDispatcher>([this](Core::Key key)
																	   { this->KeyHandler(key); });
			_dispatcher->Listen(linux_device_id);
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
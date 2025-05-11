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

			if (!context.IsKeySuppressed())
			{
				context.result.keys.insert(context.result.keys.begin(), key);
			}

			_dispatcher->Send(context.result.keys);
			context.state.OnLateKeyEvent(key);
		}

		void OnKey(Core::KeyContext& context) override
		{
			next(context);
		}
	};
}
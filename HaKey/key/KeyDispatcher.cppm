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
		std::shared_ptr<Core::KeyResult> result = std::make_shared<Core::KeyResult>();

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
			result->Clear();
			OnKey(key, result);

			if (!result->suppress_original)
			{
				result->keys.insert(result->keys.begin(), key);
			}

			_dispatcher->Send(result->keys);
		}

		void OnKey(Core::Key key, std::shared_ptr<Core::KeyResult> result) override
		{
			next(key, result);
		}
	};
}
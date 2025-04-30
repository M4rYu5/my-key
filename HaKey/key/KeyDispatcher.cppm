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
		System::ISystemKeyDispatcher *_dispatcher = nullptr;

	public:
		void Listen()
		{
			_dispatcher = new System::LinuxKeyDispatcher([this](Core::KeyEvent key)
														 { this->KeyHandler(key); });
			// todo: Create a way to pass the device id for LinuxKeyDispatcher.Listen function
			_dispatcher->Listen();
		}

	private:
		void KeyHandler(Core::KeyEvent key)
		{
			// todo: make this member variable, and add a function to clear it
			std::shared_ptr<Core::KeyResult> result = std::make_shared<Core::KeyResult>();

			OnKey(key, result);

			if (!result->suppress_original)
			{
				result->keys.insert(result->keys.begin(), key);
			}

			_dispatcher->Send(result->keys);
		}

		void OnKey(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result) override
		{
			next(key, result);
		}

	public:
		~KeyDispatcher()
		{
			delete _dispatcher;
		}
	};
}
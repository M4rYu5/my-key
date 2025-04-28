module;

#include <cstring>
#include <vector>
#include <span>
#include <functional>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>

export module System:System;

export import Core;

namespace HaKey::System
{

	export class ISystemKeyDispatcher
	{
    protected:
        std::function<void(Core::KeyEvent)> on_key = nullptr;

	public:
		ISystemKeyDispatcher(std::function<void(Core::KeyEvent)> on_key)
		{
			this->on_key = on_key;
		}

		virtual void Listen(int device_id = 0) = 0;

		virtual void Send(std::span<Core::KeyEvent> events) = 0;
        
        virtual ~ISystemKeyDispatcher() = default;
	};
}
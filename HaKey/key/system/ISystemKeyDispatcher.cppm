module;

#include <cstring>
#include <vector>
#include <span>
#include <functional>


export module System:System;

import Core;

namespace HaKey::System
{

	export class ISystemKeyDispatcher
	{
    protected:
        std::function<void(Core::Key)> on_key = nullptr;

	public:
		ISystemKeyDispatcher(std::function<void(Core::Key)> on_key)
		{
			this->on_key = on_key;
		}

		virtual void Listen(int device_id = 0) = 0;

		virtual void Send(std::span<Core::Key> events) = 0;

        virtual ~ISystemKeyDispatcher() = default;
	};
}
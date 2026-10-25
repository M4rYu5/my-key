module;

#include <functional>
#include <span>
#include <utility>

export module System:System;

import Core;

namespace HaKey::System
{
	export class ISystemKeyDispatcher
	{
	protected:
		std::function<void(Core::Key)> on_key;

	public:
		ISystemKeyDispatcher(std::function<void(Core::Key)> on_key) : on_key(std::move(on_key)) {}

		virtual void Listen() = 0;

		virtual void Send(std::span<const Core::Key> events) = 0;

		virtual ~ISystemKeyDispatcher() = default;
	};
}

module;

#if LINUX

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <span>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#endif

export module System:Linux;

#if LINUX

import Core;
import :System;

namespace HaKey::System
{

	export class LinuxKeyDispatcher : public ISystemKeyDispatcher
	{
	private:
		const struct input_event sync_ev{.time = {}, .type = EV_SYN, .code = SYN_REPORT, .value = 0};
		int uinput_fd = setup_uinput_device();
		std::vector<input_event> ev_batch;

	public:
		LinuxKeyDispatcher(std::function<void(Core::Key)> on_key) : ISystemKeyDispatcher(on_key)
		{
			ev_batch.reserve(100);
		}

		void Listen(int device_id = 0)
		{
			std::string dev_path = "/dev/input/event" + std::to_string(device_id); // your input device
			int input_fd = open(dev_path.c_str(), O_RDONLY);
			if (input_fd < 0)
			{
				std::cerr << "Failed to open device\n";
				return;
			}

			if (ioctl(input_fd, EVIOCGRAB, 1) < 0)
			{
				std::cerr << "Failed to grab input device\n";
				return;
			}

			struct input_event ev;
			while (true)
			{
				ssize_t bytes = read(input_fd, &ev, sizeof(ev));
				if (bytes == sizeof(ev))
				{
					if (ev.type != EV_KEY)
					{
						// forward everything that isn't a key to the virtual keyboard
						write(uinput_fd, &ev, sizeof(ev));
						continue;
					}

					// pass it into the filter
					if (on_key)
					{
						on_key(Core::Key(ev.code, ev.value));
					}
				}
			}

			ioctl(input_fd, EVIOCGRAB, 0);
			close(input_fd);
			return;
		}

		void Send(std::span<Core::Key> events)
		{
			ev_batch.clear();
			for (const Core::Key &key : events)
			{
				ev_batch.push_back({
					.time = {},
					.type = EV_KEY,
					.code = key.code,
					.value = key.state
				});
			}
			// add sync event
			ev_batch.push_back(sync_ev);
			// write events
			write(uinput_fd, ev_batch.data(), ev_batch.size() * sizeof(input_event));
		}

	private:
		int setup_uinput_device()
		{
			int ufd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
			if (ufd < 0)
				return -1;

			ioctl(ufd, UI_SET_EVBIT, EV_KEY);
			for (int i = 0; i < 256; ++i)
				ioctl(ufd, UI_SET_KEYBIT, i);

			struct uinput_setup usetup = {};
			usetup.id.bustype = BUS_USB;
			strcpy(usetup.name, "HaKey Virtual Keyboard");

			ioctl(ufd, UI_DEV_SETUP, &usetup);
			ioctl(ufd, UI_DEV_CREATE);

			if (ufd < 0)
			{
				std::cerr << "Failed to set up uinput device\n";
			}

			return ufd;
		}

	public:
		~LinuxKeyDispatcher()
		{
			close(uinput_fd);
		}
	};

}

#endif

module;

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

export module System:Linux;

import Core;
import :System;

namespace HaKey::System
{

	export class LinuxKeyDispatcher : public ISystemKeyDispatcher
	{
	private:
		int uinput_fd = setup_uinput_device();

	public:
		LinuxKeyDispatcher(std::function<void(Core::KeyEvent)> on_key) : ISystemKeyDispatcher(on_key) {}

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
					if (on_key){
						on_key(Core::KeyEvent(ev.code, ev.value));
					}
				}
			}

			ioctl(input_fd, EVIOCGRAB, 0);
			close(input_fd);
			return;
		}

		void Send(std::span<Core::KeyEvent> events)
		{
			struct input_event ev;

			for (const Core::KeyEvent& key : events ){
				ev.type = EV_KEY;
				ev.code = key.key_code;
				ev.value = key.state;
				write(uinput_fd, &ev, sizeof(ev));
			}
			
			// sync event
			ev.type = EV_SYN;
			ev.code = SYN_REPORT;
			ev.value = 0;
			write(uinput_fd, &ev, sizeof(ev));
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
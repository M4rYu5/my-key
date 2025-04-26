module;

#include <memory>
#include <iostream>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>

export module System:Linux;

import Core;

namespace HaKey::System
{

	export class LinuxKeyDispatcher
	{
	private:
		void (*on_key)(std::shared_ptr<Core::KeyEvent>);
		int uinput_fd = setup_uinput_device();

	public:
		LinuxKeyDispatcher(void (&on_key)(std::shared_ptr<Core::KeyEvent>))
		{
			this->on_key = on_key;
		}

		void Listen()
		{
			const char *dev_path = "/dev/input/event0"; // your input device
			int input_fd = open(dev_path, O_RDONLY);
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
					if (ev.type == EV_KEY && ev.code == 30)
					{
						std::cout << "Suppressed A key\n";
						continue; // don't forward it
					}

					// Forward everything else to virtual device
					Send((Core::VKey)ev.code);
				}
			}

			ioctl(input_fd, EVIOCGRAB, 0);
			close(input_fd);
			return;
		}

		void Send(Core::VKey k)
		{
			struct input_event ev;

			// key press
			ev.type = EV_KEY;
			ev.code = (__u16)k;
			ev.value = 1;
			write(uinput_fd, &ev, sizeof(ev));

			// key release
			ev.type = EV_KEY;
			ev.code = (__u16)k;
			ev.value = 0;
			write(uinput_fd, &ev, sizeof(ev));

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
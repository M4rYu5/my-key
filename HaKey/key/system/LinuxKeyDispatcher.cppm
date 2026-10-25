module;

#if LINUX

#include <cerrno>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <unistd.h>

#endif

export module System:Linux;

#if LINUX

import Core;
import :System;
import :LinuxInputDevice;
import :LinuxSettings;

namespace HaKey::System
{
	export class LinuxKeyDispatcher : public ISystemKeyDispatcher
	{
	private:
		enum class ListenResult
		{
			ChangeDevice,
			Disconnected
		};

		const struct input_event sync_ev{.time = {}, .type = EV_SYN, .code = SYN_REPORT, .value = 0};
		int input_fd = -1;
		int uinput_fd = -1;
		std::vector<input_event> ev_batch;
		std::function<void()> reset_state;

	public:
		LinuxKeyDispatcher(std::function<void(Core::Key)> on_key, std::function<void()> reset_state)
			: ISystemKeyDispatcher(std::move(on_key)), reset_state(std::move(reset_state))
		{
			ev_batch.reserve(100);
		}

		void Listen() override
		{
			auto mode = DeviceSelectionMode::Automatic;
			while (true)
			{
				const auto device = SelectInputDevice(mode);
				if (!device)
					return;
				if (!open_input_device(*device, mode))
					continue;
				std::cout << "Listening to " << device->name << " (" << device->path << ")\n"
				          << "q + Enter: change device\n" << std::flush;

				const auto result = listen_to_device(*device);
				close_devices();
				reset_state();
				if (result == ListenResult::Disconnected)
					std::cout << "Device \"" << device->name << "\" was disconnected.\n";
				mode = result == ListenResult::ChangeDevice ? DeviceSelectionMode::Manual : DeviceSelectionMode::Refresh;
			}
		}

		void Send(std::span<const Core::Key> events) override
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
			ev_batch.push_back(sync_ev);
			write(uinput_fd, ev_batch.data(), ev_batch.size() * sizeof(input_event));
		}

	private:
		static bool device_missing(int error)
		{
			return error == ENOENT || error == ENODEV || error == ENXIO || error == EIO;
		}

		bool input_setup_failed(const std::string &action, const InputDevice &device, int error)
		{
			close_devices();
			if (!device_missing(error))
				ThrowDeviceError(action, device.path, error);
			std::cout << "The selected device disappeared. Please select again.\n";
			return false;
		}

		bool open_input_device(const InputDevice &device, DeviceSelectionMode &mode)
		{
			input_fd = open(device.path.c_str(), O_RDONLY | O_CLOEXEC);
			if (input_fd < 0)
				return input_setup_failed("Cannot open input device", device, errno);

			// An event number may have been reused while the menu was open.
			std::vector<char> name(device.name.size() + 2, '\0');
			if (ioctl(input_fd, EVIOCGNAME(name.size() - 1), name.data()) < 0)
				return input_setup_failed("Cannot read input device name", device, errno);
			if (device.name != name.data())
			{
				close_devices();
				std::cout << "The selected device changed. Please select again.\n";
				return false;
			}
			SaveInputDevice(device.name);
			setup_uinput_device();

			while (is_key_pressed(input_fd, KEY_ENTER))
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

			if (ioctl(input_fd, EVIOCGRAB, 1) < 0)
			{
				const int error = errno;
				if (device_missing(error))
					mode = DeviceSelectionMode::Refresh;
				return input_setup_failed("Cannot grab input device", device, error);
			}
			return true;
		}

		ListenResult listen_to_device(const InputDevice &device)
		{
			TerminalInput terminal;
			while (true)
			{
				pollfd descriptors[] = {{input_fd, POLLIN, 0}, {terminal.Descriptor(), POLLIN, 0}};
				if (poll(descriptors, 2, -1) < 0)
				{
					if (errno == EINTR)
						continue;
					ThrowDeviceError("Cannot wait for input from", device.path, errno);
				}
				if (descriptors[0].revents & (POLLHUP | POLLERR))
					return ListenResult::Disconnected;
				if (descriptors[0].revents & POLLNVAL)
					ThrowDeviceError("Invalid input descriptor for", device.path, EBADF);
				if (descriptors[0].revents & POLLIN)
				{
					struct input_event ev;
					const ssize_t bytes = read(input_fd, &ev, sizeof(ev));
					if (bytes < 0 && errno == EINTR)
						continue;
					if (bytes == 0 || (bytes < 0 && device_missing(errno)))
						return ListenResult::Disconnected;
					if (bytes != sizeof(ev))
						ThrowDeviceError("Cannot read input device", device.path, bytes < 0 ? errno : EIO);
					if (ev.type != EV_KEY)
						write(uinput_fd, &ev, sizeof(ev));
					else if (on_key)
						on_key(Core::Key(ev.code, ev.value));
				}
				if (descriptors[1].revents & POLLNVAL)
					throw std::runtime_error("Cannot read terminal input: invalid stdin.");
				if (descriptors[1].revents & (POLLIN | POLLHUP | POLLERR))
				{
					const auto command = terminal.ReadLine();
					if (command && (*command == "q" || *command == "Q"))
						return ListenResult::ChangeDevice;
				}
			}
		}

		void close_devices()
		{
			if (input_fd >= 0)
			{
				ioctl(input_fd, EVIOCGRAB, 0);
				close(input_fd);
				input_fd = -1;
			}
			if (uinput_fd >= 0)
			{
				ioctl(uinput_fd, UI_DEV_DESTROY);
				close(uinput_fd);
				uinput_fd = -1;
			}
		}

		bool is_key_pressed(int fd, unsigned int key_code)
		{
			char key_states[KEY_MAX / 8 + 1]{};
			if (ioctl(fd, EVIOCGKEY(sizeof(key_states)), key_states) < 0)
				return false;
			return (key_states[key_code / 8] & (1 << (key_code % 8))) != 0;
		}

		void setup_uinput_device()
		{
			uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK | O_CLOEXEC);
			if (uinput_fd < 0)
				ThrowDeviceError("Cannot open virtual keyboard device", "/dev/uinput", errno);

			if (ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY) < 0)
				ThrowDeviceError("Cannot enable keyboard events on", "/dev/uinput", errno);
			for (int i = 0; i < 256; ++i)
			{
				if (ioctl(uinput_fd, UI_SET_KEYBIT, i) < 0)
					ThrowDeviceError("Cannot enable key on", "/dev/uinput", errno);
			}

			struct uinput_setup usetup = {};
			usetup.id.bustype = BUS_USB;
			strcpy(usetup.name, VirtualKeyboardName);
			if (ioctl(uinput_fd, UI_DEV_SETUP, &usetup) < 0 || ioctl(uinput_fd, UI_DEV_CREATE) < 0)
				ThrowDeviceError("Cannot create virtual keyboard on", "/dev/uinput", errno);
		}

	public:
		~LinuxKeyDispatcher() override
		{
			close_devices();
		}
	};
}

#endif

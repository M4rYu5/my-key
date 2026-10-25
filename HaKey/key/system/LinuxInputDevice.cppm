module;

#if LINUX
#include <algorithm>
#include <cerrno>
#include <charconv>
#include <chrono>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <poll.h>
#include <unistd.h>
#endif

export module System:LinuxInputDevice;

#if LINUX
import :LinuxSettings;

namespace HaKey::System
{
	export inline constexpr char VirtualKeyboardName[] = "HaKey Virtual Keyboard";

	export void ClearTerminal()
	{
		if (isatty(STDOUT_FILENO))
			std::cout << "\033[2J\033[H" << std::flush;
	}

	export struct InputDevice
	{
		std::string name;
		std::string path;
		std::string persistent_path;

		bool operator==(const InputDevice &) const = default;
	};

	export enum class DeviceSelectionMode
	{
		Automatic,
		Refresh,
		Manual
	};

	namespace
	{
		constexpr auto refresh_interval = std::chrono::seconds(5);

		std::string trim(const std::string &value)
		{
			const auto first = value.find_first_not_of(" \t\r\n");
			if (first == std::string::npos)
				return {};
			return value.substr(first, value.find_last_not_of(" \t\r\n") - first + 1);
		}

		void assign_persistent_paths(std::vector<InputDevice> &devices, const std::filesystem::path &directory)
		{
			for (auto &device : devices)
			{
				if (!device.persistent_path.empty())
					continue;
				std::error_code error;
				std::string preferred_path;
				for (const auto &entry : std::filesystem::directory_iterator(directory, error))
				{
					const auto target = std::filesystem::canonical(entry.path(), error);
					if (error || target != device.path)
						continue;
					const auto path = entry.path().string();
					if (preferred_path.empty() || path < preferred_path)
						preferred_path = path;
				}
				device.persistent_path = preferred_path;
			}
		}

		std::vector<InputDevice> available_devices()
		{
			std::vector<InputDevice> devices;
			for (const auto &entry : std::filesystem::directory_iterator("/sys/class/input"))
			{
				const auto event = entry.path().filename().string();
				if (!event.starts_with("event"))
					continue;
				std::ifstream name_file(entry.path() / "device/name");
				std::string name;
				if (!std::getline(name_file, name) || name.empty() || name == VirtualKeyboardName)
					continue;
				const auto path = "/dev/input/" + event;
				std::error_code error;
				if (std::filesystem::exists(path, error))
					devices.push_back({name, path, {}});
			}
			assign_persistent_paths(devices, "/dev/input/by-id");
			assign_persistent_paths(devices, "/dev/input/by-path");
			std::erase_if(devices, [](const auto &device) { return device.persistent_path.empty(); });
			std::sort(devices.begin(), devices.end(), [](const auto &a, const auto &b)
			{
				return a.name != b.name ? a.name < b.name : a.path < b.path;
			});
			return devices;
		}

		const InputDevice *find_saved_device(const std::vector<InputDevice> &devices, const std::string &path)
		{
			for (const auto &device : devices)
			{
				if (!path.empty() && device.persistent_path == path)
					return &device;
			}
			return nullptr;
		}

		void show_devices(const std::vector<InputDevice> &devices, bool refreshing)
		{
			if (refreshing)
				std::cout << "\nDevices (refresh: " << refresh_interval.count() << "s):\n";
			else
				std::cout << "\nDevices (manual):\n";
			for (std::size_t i = 0; i < devices.size(); ++i)
				std::cout << "  " << i + 1 << ") " << devices[i].name << " (" << devices[i].path << ")\n";
			if (devices.empty())
				std::cout << (refreshing ? "  No devices available. Waiting for a device to appear.\n"
				                         : "  No devices available.\n");
			std::cout << (refreshing ? "Device number, or q for manual selection (Enter): "
			                         : "Device number, r: refresh, q: quit (Enter): ") << std::flush;
		}
	}

	// Call ReadLine only after poll reports that Descriptor is ready.
	export class TerminalInput
	{
		std::string buffer;
		bool open = true;

	public:
		int Descriptor() const { return open ? STDIN_FILENO : -1; }
		bool IsOpen() const { return open; }
		void Clear() { buffer.clear(); }

		std::optional<std::string> ReadLine()
		{
			char character;
			const auto bytes = read(STDIN_FILENO, &character, 1);
			if (bytes < 0 && errno == EINTR)
				return std::nullopt;
			if (bytes < 0)
				throw std::runtime_error("Cannot read terminal input: " + std::string(std::strerror(errno)));
			if (bytes == 0)
			{
				open = false;
				return std::nullopt;
			}
			if (character != '\n')
			{
				buffer += character;
				return std::nullopt;
			}
			auto line = trim(buffer);
			buffer.clear();
			return line;
		}
	};

	export std::optional<InputDevice> SelectInputDevice(DeviceSelectionMode &mode)
	{
		const auto saved_path = LoadInputDevicePath();
		auto devices = available_devices();
		const auto saved = find_saved_device(devices, saved_path);

		if (mode == DeviceSelectionMode::Automatic)
		{
			if (saved)
				return *saved;
			mode = saved_path.empty() ? DeviceSelectionMode::Manual : DeviceSelectionMode::Refresh;
		}

		if (saved_path.empty())
			std::cout << "No input device configured.\n";
		else if (mode == DeviceSelectionMode::Refresh)
			std::cout << "Waiting for \"" << saved_path << "\" (auto-reconnect).\n";
		show_devices(devices, mode == DeviceSelectionMode::Refresh);

		TerminalInput terminal;
		auto next_refresh = std::chrono::steady_clock::now() + refresh_interval;
		while (true)
		{
			const bool refreshing = mode == DeviceSelectionMode::Refresh;
			if (refreshing && std::chrono::steady_clock::now() >= next_refresh)
			{
				auto refreshed = available_devices();
				const auto match = find_saved_device(refreshed, saved_path);
				if (match)
					return *match;
				const bool changed = refreshed != devices;
				if (changed || isatty(STDOUT_FILENO))
				{
					ClearTerminal();
					if (changed)
					{
						std::cout << "Device list changed. Select again.\n";
						terminal.Clear();
					}
					devices = std::move(refreshed);
					std::cout << "Waiting for \"" << saved_path << "\" (auto-reconnect).\n";
					show_devices(devices, true);
				}
				next_refresh = std::chrono::steady_clock::now() + refresh_interval;
			}

			// Keep checking for the saved device even when launched without a terminal.
			const auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(next_refresh - std::chrono::steady_clock::now());
			pollfd terminal_poll{terminal.Descriptor(), POLLIN, 0};
			const int timeout = refreshing ? std::max(0, static_cast<int>(remaining.count())) : -1;
			const int ready = poll(&terminal_poll, 1, timeout);
			if (ready < 0)
			{
				if (errno == EINTR)
					continue;
				throw std::runtime_error("Cannot read terminal input: " + std::string(std::strerror(errno)));
			}
			if (ready == 0)
				continue;
			if (terminal_poll.revents & POLLNVAL)
				throw std::runtime_error("Cannot read terminal input: invalid stdin.");

			const auto input = terminal.ReadLine();
			if (!terminal.IsOpen())
			{
				if (!refreshing || saved_path.empty())
				{
					std::cout << "\nRun HaKey in a terminal to select an input device.\n";
					return std::nullopt;
				}
				std::cout << "\nTerminal input closed; waiting for the saved device.\n";
				continue;
			}
			if (!input)
				continue;
			const bool quit = *input == "q" || *input == "Q";
			const bool refresh_once = *input == "r" || *input == "R";
			if (quit && !refreshing)
			{
				ClearTerminal();
				return std::nullopt;
			}
			if (quit || (!refreshing && refresh_once))
			{
				mode = DeviceSelectionMode::Manual;
				devices = available_devices();
				ClearTerminal();
				show_devices(devices, false);
				continue;
			}
			std::size_t choice = 0;
			const auto [end, error] = std::from_chars(input->data(), input->data() + input->size(), choice);
			if (error == std::errc{} && end == input->data() + input->size() && choice > 0 && choice <= devices.size())
			{
				mode = DeviceSelectionMode::Manual;
				return devices[choice - 1];
			}
			std::cout << "Invalid selection. Try again: " << std::flush;
		}
	}

	export [[noreturn]] void ThrowDeviceError(const std::string &action, const std::string &path, int error)
	{
		std::ostringstream message;
		message << action << " " << path << ": " << std::strerror(error);
		if (error == EACCES || error == EPERM)
			message << "\nHaKey needs read access to the input device and write access to /dev/uinput.";
		else if (path == "/dev/uinput" && error == ENOENT)
			message << "\nLoad the uinput kernel module with: sudo modprobe uinput";
		throw std::runtime_error(message.str());
	}
}
#endif

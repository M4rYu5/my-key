module;

#if LINUX
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <pwd.h>
#include <unistd.h>
#endif

export module System:LinuxSettings;

#if LINUX
namespace HaKey::System
{
	namespace
	{
		std::string trim_setting(const std::string &value)
		{
			const auto first = value.find_first_not_of(" \t\r\n");
			if (first == std::string::npos)
				return {};
			return value.substr(first, value.find_last_not_of(" \t\r\n") - first + 1);
		}

		std::filesystem::path config_path()
		{
			if (const char *xdg = std::getenv("XDG_CONFIG_HOME"); xdg && std::filesystem::path(xdg).is_absolute())
				return std::filesystem::path(xdg) / "HaKey/settings.ini";

			const char *home = std::getenv("HOME");
			if (!home || !std::filesystem::path(home).is_absolute())
			{
				const auto *user = getpwuid(getuid());
				home = user ? user->pw_dir : nullptr;
			}
			if (!home)
				throw std::runtime_error("Cannot find your home directory for HaKey settings.");
			return std::filesystem::path(home) / ".config/HaKey/settings.ini";
		}
	}

	export std::string LoadInputDevicePath()
	{
		const auto path = config_path();
		if (!std::filesystem::exists(path))
			return {};
		std::ifstream settings(path);
		if (!settings)
			throw std::runtime_error("Cannot read settings: " + path.string());

		std::string section, line;
		while (std::getline(settings, line))
		{
			line = trim_setting(line);
			if (line.empty() || line.front() == '#' || line.front() == ';')
				continue;
			if (line.front() == '[' && line.back() == ']')
			{
				section = trim_setting(line.substr(1, line.size() - 2));
				continue;
			}
			const auto separator = line.find('=');
			if (section != "input" || separator == std::string::npos || trim_setting(line.substr(0, separator)) != "device_path")
				continue;

			const auto value = trim_setting(line.substr(separator + 1));
			if (value.empty() || value.front() != '"')
				return value;
			std::istringstream quoted_value(value);
			std::string device_path;
			if (quoted_value >> std::quoted(device_path))
				return device_path;
			std::cerr << "Invalid device_path in " << path << ". Please select a device again.\n";
			return {};
		}
		if (settings.bad())
			throw std::runtime_error("Cannot read settings: " + path.string());
		return {};
	}

	export void SaveInputDevice(const std::string &device_path)
	{
		if (LoadInputDevicePath() == device_path)
			return;
		const auto path = config_path();
		std::filesystem::create_directories(path.parent_path());
		const auto temporary = path.string() + ".tmp";
		std::ofstream settings(temporary);
		settings << "[input]\ndevice_path = " << std::quoted(device_path) << '\n';
		settings.close();
		if (!settings)
			throw std::runtime_error("Cannot save settings: " + path.string());
		std::filesystem::rename(temporary, path);
	}
}
#endif

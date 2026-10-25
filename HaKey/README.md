# HaKey

HaKey is a C++ alternative to the personal AutoHotkey scripts in this repository.
It supports Linux and Windows. macOS support is not planned.

## Stack and layers

- C++20 with modules, built using CMake and Clang or Visual Studio on Windows.
- Naming conventions are closer to C# than conventional C++.
- Layers are hard-coded for personal use:
  - [HomeRowNavigation.cppm](key/layers/HomeRowNavigation.cppm) implements
    [homeRowNavigation.ahk](../scripts/homeRowNavigation.ahk).
  - [60KeyRemap.cppm](key/layers/60KeyRemap.cppm) implements
    [60pKeyRemap.ahk](../scripts/60pKeyRemap.ahk).
  - [StringReplaceLayer.cppm](key/layers/StringReplaceLayer.cppm) provides
    hotstring replacements.

Linux device selection is configurable as described below.

## Build and run

Run these commands from the `HaKey` directory. The project requires CMake 3.30
or newer and a compiler supporting C++20 modules. The presets use Ninja.

### Install dependencies

- **Ubuntu-based distributions (APT)**

  ```sh
  sudo apt install cmake clang clang-tools ninja-build
  ```

- **Arch Linux (pacman)**

  ```sh
  sudo pacman -Syu --needed cmake clang ninja
  ```

### Linux debug build

```sh
cmake --preset linux-debug
cmake --build --preset linux-debug
./out/build/linux-debug/HaKey
```

### Release builds

| Platform and compiler | Configure | Build |
| --- | --- | --- |
| Windows (Visual Studio) | `cmake --preset x64-release` | `cmake --build --preset x64-release` |
| Windows (Clang) | `cmake --preset x64-release-clang` | `cmake --build --preset x64-release-clang` |
| Linux (Clang) | `cmake --preset linux-release` | `cmake --build --preset linux-release` |

Run the Linux release build with `./out/build/linux-release/HaKey`.

## Linux settings

Settings are saved in `$XDG_CONFIG_HOME/HaKey/settings.ini`
(default: `~/.config/HaKey/settings.ini`).

```ini
[input]
device_path = "/dev/input/by-id/usb-BY_Tech_Gaming_Keyboard-event-kbd"
```

## Linux troubleshooting

### Input and uinput permissions

HaKey needs read access to the selected `/dev/input/eventN` device and write
access to `/dev/uinput`. If access is denied, it reports the required permissions.

For manual device inspection, use `cat /proc/bus/input/devices`. The optional
`input-utils` package also provides `input-events` to probe individual devices.

#### Run with sudo

You can run HaKey with `sudo`:

```sh
sudo ./out/build/linux-debug/HaKey
```

Running with `sudo` uses that process's configuration directory.

#### Run as your normal user

Create a dedicated group to run HaKey without `sudo` and keep your normal user
configuration:

```sh
sudo groupadd -f hakey
sudo usermod -aG hakey "$USER"
```

Create `/etc/udev/rules.d/99-hakey.rules` with:

```udev
SUBSYSTEM=="input", KERNEL=="event*", GROUP="hakey", MODE="0660"
SUBSYSTEM=="misc", KERNEL=="uinput", GROUP="hakey", MODE="0660"
```

Reload the rules and apply them to existing devices:

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger --subsystem-match=input
sudo udevadm trigger --subsystem-match=misc --sysname-match=uinput
```

Log out and back in to apply group membership, then run HaKey again. These rules
allow group members to read all input events and write to the virtual input
device.

### Missing `/dev/uinput`

Try loading the kernel module:

```sh
sudo modprobe uinput
```

If the module is unavailable, use or install a kernel containing `uinput`.
On Kubuntu 25.04, the missing module may be supplied by the extra kernel modules
package:

```sh
sudo apt install linux-modules-extra-$(uname -r)
```

Reboot after installing the modules. Updating packages with
`sudo apt update && sudo apt upgrade` and rebooting can also help if an updated
kernel includes the missing module.

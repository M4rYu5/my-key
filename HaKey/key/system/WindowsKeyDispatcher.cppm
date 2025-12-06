module;
#if WINDOWS

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <span>
#include <vector>

#include <windows.h>
#endif

export module System:Windows;

#if WINDOWS
import :System;
import KeyState;
import Core;



namespace HaKey::System
{
	namespace WindowsHooks {
		// Keyboard hook handle
		HHOOK hKeyboardHook;
		std::function<void(Core::Key)> on_key;

		// Low-level keyboard hook procedure
		LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
			if (nCode == HC_ACTION) {
				KBDLLHOOKSTRUCT* pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;

				if (!(pKeyBoard->flags & LLKHF_INJECTED /*Ignore events sent by SendInput to avoid recursion*/)
					&& (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP)) {
					HaKey::Core::Key key((int)pKeyBoard->vkCode, wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN ? KeyState::Down : KeyState::Up);
					if (on_key)
						on_key(key);
					return 1;
				}
			}

			return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
		}
	} //namespace WindowsHooks

	export class WindowsKeyDispatcher : public ISystemKeyDispatcher
	{
	private:
		std::vector<INPUT> ev_batch;

	public:
		/// @brief Only one instance at a time is supported.
		WindowsKeyDispatcher(std::function<void(Core::Key)> on_key) : ISystemKeyDispatcher(on_key)
		{
			ev_batch.reserve(100);
			WindowsHooks::on_key = on_key;
		}

		void Listen(int device_id = 0)
		{
			// Install the hook
			WindowsHooks::hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, WindowsHooks::LowLevelKeyboardProc, NULL, 0);

			if (WindowsHooks::hKeyboardHook == NULL) {
				std::cerr << "Failed to hook!" << std::endl;
				return;
			}

			// Run message loop
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// I don't think it will reach this line of code.
			UnhookWindowsHookEx(WindowsHooks::hKeyboardHook);
			return;
		}

		void Send(std::span<Core::Key> events)
		{
			ev_batch.clear();
			for (const Core::Key& key : events)
			{
				INPUT input = {};
				input.type = INPUT_KEYBOARD;
				input.ki.wVk = key.code;
				input.ki.dwFlags = (key.state == KeyState::Up) ? KEYEVENTF_KEYUP : 0;

				ev_batch.push_back(input);
			}
			// write events
			SendInput(ev_batch.size(), ev_batch.data(), sizeof(INPUT));
		}

	private:


	public:
		~WindowsKeyDispatcher()
		{
			WindowsHooks::on_key = nullptr;
		}



	};

}

#endif
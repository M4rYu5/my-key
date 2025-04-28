module;

#include <memory>
#include <iostream>
#include <cstring>


#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>

export module Dispatcher;

import Core;
import System;

namespace HaKey {

	export class KeyDispatcher : public Core::KeyChainHandler {
	private: 
		System::ISystemKeyDispatcher* _dispatcher = nullptr;

	public:
		void Listen() {
			_dispatcher = new System::LinuxKeyDispatcher([this](Core::KeyEvent key)
			{
				this->KeyHandler(key);
			});
		}

	private:
		void KeyHandler(Core::KeyEvent key){
			
		}
		
		void OnKey(Core::KeyEvent key, std::shared_ptr<Core::KeyResult> result) override {
			next->OnKey(key, result);
		}

		~KeyDispatcher(){
			delete _dispatcher;
		}
	};
}
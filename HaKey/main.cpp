// TODO: Delete /lib/libevdev library. I don't think I'm gonna need it anymore.

import System;


int main() {
	System::LinuxKeyDispatcher d = new System::LinuxKeyDispatcher(nullptr);
	d.Listen();
}

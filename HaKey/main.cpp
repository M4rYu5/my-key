// TODO: Delete /lib/libevdev library. I don't think I'm gonna need it anymore.

import Key;

int main() {

}


//#include <iostream>
//#include <fcntl.h>
//#include <unistd.h>
//#include <linux/input.h>
//#include <linux/uinput.h>
//#include <sys/ioctl.h>
//#include <cstring>
//
//
//
//// Forward declaration
//int setup_uinput_device();
//
//int main() {
//    const char* dev_path = "/dev/input/event0";  // your input device
//    int input_fd = open(dev_path, O_RDONLY);
//    if (input_fd < 0) {
//        std::cerr << "Failed to open device\n";
//        return 1;
//    }
//
//    if (ioctl(input_fd, EVIOCGRAB, 1) < 0) {
//        std::cerr << "Failed to grab input device\n";
//        return 1;
//    }
//
//    int uinput_fd = setup_uinput_device();
//    if (uinput_fd < 0) {
//        std::cerr << "Failed to set up uinput device\n";
//        return 1;
//    }
//
//    struct input_event ev;
//    while (true) {
//        ssize_t bytes = read(input_fd, &ev, sizeof(ev));
//        if (bytes == sizeof(ev)) {
//            if (ev.type == EV_KEY && ev.code == 30) {
//                std::cout << "Suppressed A key\n";
//                continue;  // don't forward it
//            }
//
//            // Forward everything else to virtual device
//            write(uinput_fd, &ev, sizeof(ev));
//        }
//    }
//
//    ioctl(input_fd, EVIOCGRAB, 0);
//    close(input_fd);
//    close(uinput_fd);
//    return 0;
//}
//
//int setup_uinput_device() {
//    int ufd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
//    if (ufd < 0) return -1;
//
//    ioctl(ufd, UI_SET_EVBIT, EV_KEY);
//    for (int i = 0; i < 256; ++i)
//        ioctl(ufd, UI_SET_KEYBIT, i);
//
//    struct uinput_setup usetup = {};
//    usetup.id.bustype = BUS_USB;
//    strcpy(usetup.name, "HaKey Virtual Keyboard");
//
//    ioctl(ufd, UI_DEV_SETUP, &usetup);
//    ioctl(ufd, UI_DEV_CREATE);
//
//    return ufd;
//}
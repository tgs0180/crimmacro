#include "input.hpp"
#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <cstring>
#include <fcntl.h>
#include <cstdio>

VirtualInput::VirtualInput() : fd_(open("/dev/uinput", O_WRONLY | O_NONBLOCK)) {
    if (!valid()) return;
    ioctl(fd_.get(), UI_SET_EVBIT, EV_KEY);
    ioctl(fd_.get(), UI_SET_KEYBIT, KEY_A);
    uinput_setup setup{};
    setup.id.bustype = BUS_USB;
    setup.id.vendor = 0x1234;
    setup.id.product = 0x5678;
    std::memcpy(setup.name, "crimmacro", 10);
    int a = ioctl(fd_.get(), UI_DEV_SETUP, &setup);
    int b = ioctl(fd_.get(), UI_DEV_CREATE);
    if (a == -1 || b == -1) perror("IOCTL input.cpp");
}

bool VirtualInput::valid() const {
    return fd_.valid(); 
}

VirtualInput::~VirtualInput() {
    ioctl(fd_.get(), UI_DEV_DESTROY);
}
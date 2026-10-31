#include "input.hpp"
#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <cstring>
#include <fcntl.h>
#include <cstdio>
#include <chrono>
#include <thread>

using namespace std::chrono_literals; 

VirtualInput::VirtualInput() : fd_(open("/dev/uinput", O_WRONLY | O_NONBLOCK)) {
    if (!valid()) return;
    
    ioctl(fd_.get(), UI_SET_EVBIT, EV_ABS);
    ioctl(fd_.get(), UI_SET_ABSBIT, ABS_X);
    ioctl(fd_.get(), UI_SET_ABSBIT, ABS_Y);
    uinput_abs_setup setupX;
    setupX.code = ABS_X;
    setupX.absinfo.minimum = 0;
    setupX.absinfo.maximum = 1920;

    uinput_abs_setup setupY;
    setupY.code = ABS_Y;
    setupY.absinfo.minimum = 0;
    setupY.absinfo.maximum = 1080;

    ioctl(fd_.get(), UI_ABS_SETUP, &setupX);
    ioctl(fd_.get(), UI_ABS_SETUP, &setupY);


    ioctl(fd_.get(), UI_SET_EVBIT, EV_KEY);
    for (int key: keys) {
        ioctl(fd_.get(), UI_SET_KEYBIT, key);
    }

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

void VirtualInput::emit(int type, int code, int value) {
    input_event ev{};
    ev.type = type;
    ev.code = code;
    ev.value = value;
    write(fd_.get(), &ev, sizeof ev);
}

void VirtualInput::tap(int key) {
    emit(EV_KEY, key, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(30ms);
    emit(EV_KEY, key, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualInput::hold(int key, float seconds) {
    emit(EV_KEY, key, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    int time = static_cast<int>(seconds * 1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    emit(EV_KEY, key, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualInput::moveTo(int x, int y) {
    emit(EV_ABS, ABS_X, x);
    emit(EV_ABS, ABS_Y, y);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualInput::click() {
    tap(BTN_LEFT);
}

void VirtualInput::clickAt(int x, int y) {
    moveTo(x, y);
    std::this_thread::sleep_for(30ms);
    click();
}
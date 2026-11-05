#include "input.hpp"
#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <cstring>
#include <fcntl.h>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cmath>
#include <utility>

using namespace std::chrono_literals; 

VirtualInput::VirtualInput() : fd_(open("/dev/uinput", O_WRONLY | O_NONBLOCK)), mouseFd_(open("/dev/uinput", O_WRONLY | O_NONBLOCK)) {
    if (!valid()) return;
    
    ioctl(fd_.get(), UI_SET_EVBIT, EV_ABS);
    ioctl(fd_.get(), UI_SET_ABSBIT, ABS_X);
    ioctl(fd_.get(), UI_SET_ABSBIT, ABS_Y);
    uinput_abs_setup setupX{};
    setupX.code = ABS_X;
    setupX.absinfo.minimum = 0;
    setupX.absinfo.maximum = 1919;

    uinput_abs_setup setupY{};
    setupY.code = ABS_Y;
    setupY.absinfo.minimum = 0;
    setupY.absinfo.maximum = 1079;

    ioctl(fd_.get(), UI_ABS_SETUP, &setupX);
    ioctl(fd_.get(), UI_ABS_SETUP, &setupY);

    ioctl(fd_.get(), UI_SET_EVBIT, EV_REL);
    ioctl(fd_.get(), UI_SET_RELBIT, REL_WHEEL);


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

    // Second device for camera drags: relative movement stays apart from the absolute axes above
    ioctl(mouseFd_.get(), UI_SET_EVBIT, EV_REL);
    ioctl(mouseFd_.get(), UI_SET_RELBIT, REL_X);
    ioctl(mouseFd_.get(), UI_SET_RELBIT, REL_Y);

    ioctl(mouseFd_.get(), UI_SET_EVBIT, EV_KEY);
    ioctl(mouseFd_.get(), UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(mouseFd_.get(), UI_SET_KEYBIT, BTN_LEFT); // never pressed, but Linux only treats it as a mouse with this

    uinput_setup mouseSetup{};
    mouseSetup.id.bustype = BUS_USB;
    mouseSetup.id.vendor = 0x1234;
    mouseSetup.id.product = 0x5679;
    std::memcpy(mouseSetup.name, "crimmacro-mouse", 16);
    int c = ioctl(mouseFd_.get(), UI_DEV_SETUP, &mouseSetup);
    int d = ioctl(mouseFd_.get(), UI_DEV_CREATE);
    if (c == -1 || d == -1) perror("IOCTL input.cpp mouse");
}

bool VirtualInput::valid() const {
    return fd_.valid() && mouseFd_.valid();
}

VirtualInput::~VirtualInput() {
    ioctl(fd_.get(), UI_DEV_DESTROY);
    ioctl(mouseFd_.get(), UI_DEV_DESTROY);
}

void VirtualInput::emit(int type, int code, int value) {
    input_event ev{};
    ev.type = type;
    ev.code = code;
    ev.value = value;
    write(fd_.get(), &ev, sizeof ev);
}

void VirtualInput::emitMouse(int type, int code, int value) {
    input_event ev{};
    ev.type = type;
    ev.code = code;
    ev.value = value;
    write(mouseFd_.get(), &ev, sizeof ev);
}

void VirtualInput::tap(int key) {
    emit(EV_KEY, key, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(30ms);
    emit(EV_KEY, key, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}
// Accurate to ~1ms: sleeps towards a fixed end time, so the small oversleeps don't add up
void VirtualInput::hold(int key, float seconds) {
    emit(EV_KEY, key, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(std::lround(seconds * 1000));
    while (!shouldStop) {
        auto now = std::chrono::steady_clock::now();
        if (now >= end) break;
        // wake up at least every 10ms to check shouldStop, but never sleep past the end
        std::this_thread::sleep_until(std::min(end, now + 10ms));
    }
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

void VirtualInput::scroll(int amount) {
    emit(EV_REL, REL_WHEEL, amount);
    emit(EV_SYN, SYN_REPORT, 0);
}

void VirtualInput::drag(int dx, int dy) {
    emitMouse(EV_KEY, BTN_RIGHT, 1);
    emitMouse(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(50ms);
    emitMouse(EV_REL, REL_X, dx);
    emitMouse(EV_REL, REL_Y, dy);
    emitMouse(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(50ms);
    emitMouse(EV_KEY, BTN_RIGHT, 0);
    emitMouse(EV_SYN, SYN_REPORT, 0);
}
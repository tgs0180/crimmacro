#include "input.hpp"
#include <fcntl.h>

VirtualInput::VirtualInput() : fd_(open("/dev/uinput", O_WRONLY | O_NONBLOCK)) {


}
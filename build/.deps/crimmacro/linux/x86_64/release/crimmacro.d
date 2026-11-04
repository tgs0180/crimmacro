{
    files = {
        "build/.objs/crimmacro/linux/x86_64/release/src/camera.cpp.o",
        "build/.objs/crimmacro/linux/x86_64/release/src/eyes.cpp.o",
        "build/.objs/crimmacro/linux/x86_64/release/src/hotkeys.cpp.o",
        "build/.objs/crimmacro/linux/x86_64/release/src/hyprland.cpp.o",
        "build/.objs/crimmacro/linux/x86_64/release/src/input.cpp.o",
        "build/.objs/crimmacro/linux/x86_64/release/src/main.cpp.o",
        "build/.objs/crimmacro/linux/x86_64/release/src/moving.cpp.o"
    },
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-s",
            "-lopencv_core",
            "-lopencv_imgcodecs",
            "-lopencv_imgproc",
            "-lopencv_geometry"
        }
    }
}
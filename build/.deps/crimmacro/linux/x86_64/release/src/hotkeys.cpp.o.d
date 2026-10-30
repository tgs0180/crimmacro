{
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-std=c++17",
            "-Iinclude",
            "-DNDEBUG"
        }
    },
    files = {
        "src/hotkeys.cpp"
    },
    depfiles_format = "gcc",
    depfiles = "hotkeys.o: src/hotkeys.cpp include/hotkeys.hpp\
"
}
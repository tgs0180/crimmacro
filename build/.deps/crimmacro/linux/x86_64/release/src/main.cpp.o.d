{
    files = {
        "src/main.cpp"
    },
    depfiles_format = "gcc",
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
    depfiles = "main.o: src/main.cpp include/hotkeys.hpp\
"
}
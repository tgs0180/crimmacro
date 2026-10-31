{
    depfiles_format = "gcc",
    depfiles = "hyprland.o: src/hyprland.cpp include/hyprland.hpp include/fd.hpp\
",
    files = {
        "src/hyprland.cpp"
    },
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
    }
}
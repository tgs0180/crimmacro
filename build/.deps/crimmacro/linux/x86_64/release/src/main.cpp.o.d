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
            "-isystem",
            "/home/sjaak/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include",
            "-DNDEBUG"
        }
    },
    depfiles_format = "gcc",
    files = {
        "src/main.cpp"
    },
    depfiles = "main.o: src/main.cpp include/hotkeys.hpp include/hyprland.hpp  include/input.hpp include/fd.hpp\
"
}
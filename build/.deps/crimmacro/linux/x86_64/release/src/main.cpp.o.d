{
    depfiles = "main.o: src/main.cpp include/hotkeys.hpp\
",
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
            "/home/sjaak/.xmake/packages/i/imgui/v1.92.9+b-docking/7de164fddbf3404fb7ed0779905107de/include",
            "-isystem",
            "/home/sjaak/.xmake/packages/i/imgui/v1.92.9+b-docking/7de164fddbf3404fb7ed0779905107de/include/imgui",
            "-isystem",
            "/home/sjaak/.xmake/packages/i/imgui/v1.92.9+b-docking/7de164fddbf3404fb7ed0779905107de/include/backends",
            "-isystem",
            "/home/sjaak/.xmake/packages/i/imgui/v1.92.9+b-docking/7de164fddbf3404fb7ed0779905107de/include/misc/cpp",
            "-DNDEBUG"
        }
    },
    depfiles_format = "gcc",
    files = {
        "src/main.cpp"
    }
}
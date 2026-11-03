{
    files = {
        "src/input.cpp"
    },
    depfiles_format = "gcc",
    depfiles = "input.o: src/input.cpp include/input.hpp include/fd.hpp\
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
            "-I/usr/include/opencv5",
            "-isystem",
            "/home/sjaak/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include",
            "-DNDEBUG"
        }
    }
}
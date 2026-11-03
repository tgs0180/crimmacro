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
            "-I/usr/include/opencv5",
            "-isystem",
            "/home/sjaak/.xmake/packages/n/nlohmann_json/v3.12.0/b36e201b1206419d94ef523b409412cf/include",
            "-DNDEBUG"
        }
    },
    depfiles = "main.o: src/main.cpp include/hotkeys.hpp include/hyprland.hpp  include/input.hpp include/fd.hpp /usr/include/opencv5/opencv2/core.hpp  /usr/include/opencv5/opencv2/core/cvdef.h  /usr/include/opencv5/opencv2/core/version.hpp  /usr/include/opencv5/opencv2/core/hal/interface.h  /usr/include/opencv5/opencv2/core/cv_cpu_dispatch.h  /usr/include/opencv5/opencv2/core/base.hpp  /usr/include/opencv5/opencv2/opencv_modules.hpp  /usr/include/opencv5/opencv2/core/cvstd.hpp  /usr/include/opencv5/opencv2/core/cvstd_wrapper.hpp  /usr/include/opencv5/opencv2/core/fwddecl.hpp  /usr/include/opencv5/opencv2/core/neon_utils.hpp  /usr/include/opencv5/opencv2/core/vsx_utils.hpp  /usr/include/opencv5/opencv2/core/exception.hpp  /usr/include/opencv5/opencv2/core/check.hpp  /usr/include/opencv5/opencv2/core/traits.hpp  /usr/include/opencv5/opencv2/core/matx.hpp  /usr/include/opencv5/opencv2/core/saturate.hpp  /usr/include/opencv5/opencv2/core/fast_math.hpp  /usr/include/opencv5/opencv2/core/matx.inl.hpp  /usr/include/opencv5/opencv2/core/types.hpp  /usr/include/opencv5/opencv2/core/mat.hpp  /usr/include/opencv5/opencv2/core/bufferpool.hpp  /usr/include/opencv5/opencv2/core/mat.inl.hpp  /usr/include/opencv5/opencv2/core/persistence.hpp  /usr/include/opencv5/opencv2/core/operations.hpp  /usr/include/opencv5/opencv2/core/cvstd.inl.hpp  /usr/include/opencv5/opencv2/core/utility.hpp  /usr/include/opencv5/opencv2/core/optim.hpp  /usr/include/opencv5/opencv2/imgcodecs.hpp include/eyes.hpp  include/moving.hpp /usr/include/opencv5/opencv2/imgproc.hpp\
"
}
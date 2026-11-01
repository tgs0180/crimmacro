
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})
set_project("crimmacro")
set_languages("c++17")
add_requires("nlohmann_json")



add_rules("mode.debug", "mode.release")

target("crimmacro")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("nlohmann_json")
    add_includedirs("/usr/include/opencv5")
    add_links("opencv_core", "opencv_imgcodecs", "opencv_imgproc")



    set_rundir("$(projectdir)")
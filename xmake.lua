add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})
set_project("crimmacro")
set_languages("c++17")


add_requires("imgui", {configs = {sdl3 = true, sdl3_renderer = true}})


add_rules("mode.debug", "mode.release")

target("crimmacro")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")

    add_packages("libsdl3", "imgui", "nlohmann_json", "taglib", "stb")
    set_rundir("$(projectdir)")
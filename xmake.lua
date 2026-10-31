
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



    set_rundir("$(projectdir)")
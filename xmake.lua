option("boost", {showmenu = true,  default = true})

if has_config("boost") then
    add_requires("boost")
end

target("html-parser")
    set_kind("shared")
    set_languages("cxx17")

    add_files("src/*.cc")
    add_headerfiles("include/(html-parser/*.h)")
    
    add_includedirs("include", {public = true})
    add_includedirs("include/html-parser")

    if has_config("boost") then
        add_packages("boost")
        add_defines("USE_BOOST")
    end
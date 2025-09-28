cmake_minimum_required(VERSION 3.14)

find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
    message(STATUS "PKG_CONFIG_FOUND: ${PKG_CONFIG_FOUND}")

    pkg_check_modules(FREETYPE freetype2)
    pkg_check_modules(HARFBUZZ harfbuzz)

    get_cmake_property(variables VARIABLES)

    list(SORT variables)

    set(prefixes_to_cache FREETYPE_ HARFBUZZ_)

    foreach (var ${variables})
        set(should_cache FALSE)
        foreach (prefix ${prefixes_to_cache})
            if (var MATCHES "^${prefix}")
                set(should_cache TRUE)
                break()
            endif ()
        endforeach ()

        if (should_cache)
            set(${var} "${${var}}" CACHE INTERNAL "Converted from pkg_check_modules")
            message(STATUS "Cached variable: ${var} = ${${var}}")
        endif ()
    endforeach ()
else ()
    message(STATUS "pkg-config not found")
endif ()
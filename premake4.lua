solution("gridtop")
    configurations {"Debug", "Release"}

    targetdir("bin")

    configuration "debug"
        defines { "DEBUG" }
        flags { "Symbols" }
    configuration "release"
        defines { "NDEBUG" }
        flags { "Optimize" }

    project("gridtop")
        --uuid("...")
        kind("ConsoleApp")
        language("C++")
        links {
            "pthread",
            "boost_system",
            "boost_regex",
            "boost_filesystem",
        }
        files {
            "src/**.h",
            "src/**.cpp"
        }

        includedirs {
        }
        libdirs {
            "/usr/lib/libwnck/"
        }

        configuration { "gmake" }
            buildoptions {
                "-std=c++11",
                "-pedantic",
                "-Wall",
                "-Wextra",

                "`pkg-config --cflags libwnck-3.0`",
            }
            linkoptions {
                "`pkg-config --libs libwnck-3.0`",
            }
            --configuration { "macosx" }
            --    buildoptions { "-U__STRICT_ANSI__", "-stdlib=libc++" }
            --    linkoptions { "-stdlib=libc++" }

local folder = _OS

solution "rook"
  configurations { "Debug", "Release" }
  location(folder)
  targetdir(folder.."/bin")

  project "rook"
    kind "ConsoleApp"
    language "C"

--- Common -----------------------------------------------------------

    files { "../src/**.h", "../src/**.c" }
    files { "../lib/**.h", "../lib/**.c" }
    includedirs { "../src/" }

--- Mac OS X ---------------------------------------------------------

    if _OS == "macosx" then
      files { "../src/**.m" }
      defines { }
      includedirs {
      }
      libdirs {
      }
      links {
      }
      buildoptions {
        "-std=c89",
        "-pedantic-errors",
        "-Wall"
      }

--- Linux ------------------------------------------------------------

    elseif _OS == "linux" then
      defines { }
      links { }

--- Windows ----------------------------------------------------------

    elseif _OS == "windows" then
      defines { }
      includedirs { }
      libdirs { }
      links { }
    end

----------------------------------------------------------------------

    configuration "Debug"
      defines { "DEBUG" }
      flags { "Symbols" }
    configuration "Release"
      defines { "NDEBUG" }
      flags { "Optimize" }

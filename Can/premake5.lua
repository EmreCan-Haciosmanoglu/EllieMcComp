workspace "Can"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Can/vendor/glfw/include"

include "Can/vendor/glfw"

project "Can"
    location "Can"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "canpch.h"
    pchsource "Can/src/canpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CAN_PLATFORM_WINDOWS",
            "CAN_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "CAN_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CAN_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CAN_DIST"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Can/vendor/spdlog/include",
        "Can/src"
    }

    links
    {
        "Can"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "CAN_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CAN_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "CAN_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "CAN_DIST"
        optimize "On"
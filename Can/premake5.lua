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
IncludeDir["GLAD"] = "Can/vendor/glad/include"
IncludeDir["imgui"] = "Can/vendor/imgui"

startproject "Sandbox"

group "Dependencies"
    include "Can/vendor/glfw"
    include "Can/vendor/glad"
    include "Can/vendor/imgui"
group ""

project "Can"
    location "Can"
    kind "SharedLib"
    language "C++"
    staticruntime "Off"

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
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.imgui}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "CAN_PLATFORM_WINDOWS",
            "CAN_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
        }

    filter "configurations:Debug"
        defines "CAN_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "CAN_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "CAN_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"

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
        systemversion "latest"

        defines
        {
            "CAN_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "CAN_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "CAN_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "CAN_DIST"
        runtime "Release"
        optimize "On"
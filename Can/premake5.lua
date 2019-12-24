workspace "Can"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

	flags
	{
		"MultiProcessorCompile"
	}
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Can/vendor/glfw/include"
IncludeDir["GLAD"] = "Can/vendor/glad/include"
IncludeDir["imgui"] = "Can/vendor/imgui"
IncludeDir["glm"] = "Can/vendor/glm"
IncludeDir["stb"] = "Can/vendor/stb"

startproject "Sandbox"

group "Dependencies"
    include "Can/vendor/glfw"
    include "Can/vendor/glad"
    include "Can/vendor/imgui"
group ""

project "Can"
    location "Can"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17" 
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    pchheader "canpch.h"
    pchsource "Can/src/canpch.cpp"
    
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb/stb_image.cpp",
        "%{prj.name}/vendor/stb/stb_image.h"
    }
    
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}"
    }
    
    links
    {
        "GLFW",
        "GLAD",
        "imgui",
        "opengl32.lib"
    }
    
    filter "system:windows"
        systemversion "latest"
        
        defines
        {
            
            "GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
        }
        
    filter "configurations:Debug"
        defines "CAN_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "CAN_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "CAN_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"
        
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }
        
    includedirs
    {
        "Can/vendor/spdlog/include",
        "Can/src",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}"
    }
    
    links
    {
        "imgui",
        "Can"
    }
        
    filter "system:windows"
        systemversion "latest"
        
    filter "configurations:Debug"
        defines "CAN_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "CAN_RELEASE"
        runtime "Release"
        optimize "on"
        
    filter "configurations:Dist"
        defines "CAN_DIST"
        runtime "Release"
        optimize "on"
            
project "2048_Extended"
    location "2048_Extended"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "Can/vendor/spdlog/include",
        "Can/src",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "imgui",
        "Can"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "CAN_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "CAN_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "CAN_DIST"
        runtime "Release"
        optimize "on"
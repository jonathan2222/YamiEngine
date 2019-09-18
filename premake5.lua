workspace "YamiEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
	location "Projects/Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Build/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Build/Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Projects/%{prj.name}/src/**.h",
		"Projects/%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Projects/YamiEngine/Externals/Include",
		"Projects/YamiEngine/src"
	}

	links { "YamiEngine" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"YAMI_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "YAMI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "YAMI_RELEASE"
		optimize "On"

project "YamiEngine"
	location "Projects/YamiEngine"
	kind "StaticLib"
	language "C++"

	targetdir ("Build/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Build/Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Projects/%{prj.name}/src/**.h",
		"Projects/%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"Projects/%{prj.name}/Externals/Include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"YAMI_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "YAMI_DEBUG"
		symbols "On"
		libdirs
		{
			"Projects/%{prj.name}/Externals/Lib/Debug/spdlogd.lib"
		}

	filter "configurations:Release"
		defines "YAMI_RELEASE"
		optimize "On"
		libdirs
		{
			"Projects/%{prj.name}/Externals/Lib/Release/spdlog.lib"
		}

workspace "YamiEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "YamiEngine"
	location "YamiEngine"
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
		"%{prj.name}/Externals/Include"
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

	filter "configurations:Release"
		defines "YAMI_RELEASE"
		optimize "On"
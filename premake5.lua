workspace "YamiEngine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations { "Debug", "Release" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines { "YAMI_PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines "YAMI_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "YAMI_RELEASE"
		optimize "On"
	filter {}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

function includeGLFW()
	includedirs "Projects/YamiEngine/Externals/Include/"
end

function linkGLFW()
	libdirs "Projects/YamiEngine/Externals/Lib/%{cfg.buildcfg}"
	-- Only the StaticLibrary should link against GLFW.
	filter "kind:not StaticLib"
		links "glfw3"
	filter {} -- Reset the filters for other settings.
end

function useEngine()
	includedirs {
		"Projects/YamiEngine/Externals/Include",
		"Projects/YamiEngine/src"
	}
	links "YamiEngine"
	linkGLFW()
end

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

	useEngine()

	filter "system:windows"
		links { "opengl32" }

	filter "system:not windows"
		links { "GL" }
	
	filter {}

function includeCatch()
	includedirs "Projects/UnitTests/Include"
	defines "CATCH_CPP11_OR_GREATER"
end

project "UnitTests"
	location "Projects/UnitTests"
	kind "ConsoleApp"
	language "C++"

	targetdir ("Build/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Build/Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Projects/%{prj.name}/src/**.h",
		"Projects/%{prj.name}/src/**.cpp"
	}

	includeCatch()
	useEngine()

	filter "system:windows"
		links { "opengl32" }

	filter "system:not windows"
		links { "GL" }
	
	filter {}

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

	includeGLFW()

	filter "configurations:Debug"
		libdirs "Projects/%{prj.name}/Externals/Lib/Debug/spdlogd.lib"

	filter "configurations:Release"
		libdirs "Projects/%{prj.name}/Externals/Lib/Release/spdlog.lib"

	filter {}

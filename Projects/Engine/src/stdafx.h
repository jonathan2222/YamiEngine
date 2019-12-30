#pragma message("Compiling precompiled headers.\n")

#include <unordered_map>
#include <vector>
#include <string>

#ifdef YAMI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#include "Engine/Defines.h"
#include "Engine/Core/Logger.h"
#include "Engine/Core/Input/Config.h"

#include "Utils/InstrumentationTimer.h"
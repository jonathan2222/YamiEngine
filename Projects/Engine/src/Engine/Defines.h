#pragma once

#include <memory>
#include <cassert>
#include "Core/Logger.h"

#ifdef YAMI_DEBUG
	#define YM_ASSERT(exp, ...) {if(!(exp)){YM_LOG_CRITICAL(__VA_ARGS__);} assert(exp); }
#else
	#define YM_ASSERT(exp, ...) {assert(exp);}
#endif

#define Ptr(T) std::unique_ptr<T>
#define Ref(T) std::shared_ptr<T>

#define MakePtr(T, ...) std::make_unique<T>(__VA_ARGS__)
#define MakeRef(T, ...) std::make_shared<T>(__VA_ARGS__)
#define NOP	// No OPeration

#define TO_STR(T) #T
#define MERGE(S1, S2) S1 ## S2

// Change this to be in config file instead?
#define YM_API_GL 1
#define YM_API_DX11 2
#define YM_CURRENT_API_TYPE YM_API_GL

#define YM_CONFIG_FILE_PATH "./Resources/Config/EngineConfig.json"
#ifdef YAMI_DEBUG
#include <crtdbg.h>
#endif

#ifdef YAMI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

// ---------------- GLFW Testing -----------------
#include "Structure/Application.h"

int main(int argc, char* argv[])
{
#ifdef YAMI_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	ym::IApp* app = new Application();

	app->processArguments(argc, argv);

	app->run();

	delete app;

	return 0;
}
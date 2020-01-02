#include "Application.h"

#include <Engine/Core/Display.h>

#include "SandboxLayer2.h"

Application::Application(ym::DisplayDesc& displayDescriptor) : IApp(displayDescriptor)
{
}

Application::~Application()
{
}

void Application::processArguments(int argc, char* argv[])
{	
}

void Application::start()
{
	ym::LayerManager* layerManager = getLayerManager();
	layerManager->push(new SandboxLayer2());
}

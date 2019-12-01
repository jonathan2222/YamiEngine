#pragma once

#include <Engine/Core/IApp.h>
#include <Engine/Core/Display.h>

// TODO: Move this to engine?
class Application : public ym::IApp
{
public:
	Application(ym::DisplayDesc& displayDescriptor);
	~Application();

	void processArguments(int argc, char* argv[]) override;

	void run() override;

private:

};
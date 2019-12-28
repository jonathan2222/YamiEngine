#pragma once

#include <Engine/Core/IApp.h>

class Application : public ym::IApp
{
public:
	Application(ym::DisplayDesc& displayDescriptor);
	~Application();

	void processArguments(int argc, char* argv[]) override;

	void start() override;
};
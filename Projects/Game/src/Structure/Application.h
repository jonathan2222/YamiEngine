#pragma once

#include <Engine/Core/IApp.h>

// TODO: Move this to engine?
class Application : public ym::IApp
{
public:
	Application();
	~Application();

	void processArguments(int argc, char* argv[]) override;

	void run() override;

private:

};
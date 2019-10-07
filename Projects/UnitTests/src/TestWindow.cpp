#define CATCH_CONFIG_MAIN
#include <Catch.h>

#include <Engine/Core/Display.h>

TEST_CASE("Window Tests", "[YamiEngine]")
{
    using namespace ym;
	Display* display = Display::create({ 600, 400, "Test Window" });
    int width = display->getWidth();
	int height = display->getHeight();
    REQUIRE(width == 600);
    REQUIRE(height == 400);
	delete display;
}

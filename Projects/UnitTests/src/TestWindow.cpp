#define CATCH_CONFIG_MAIN
#include <Catch.h>

#include <Graphics/Display.h>

TEST_CASE("Window Tests", "[YamiEngine]")
{
    using namespace ym;
    Display display{ 600, 400, "Test Window" };
    auto size = display.getWindowSize();
    REQUIRE(size.first == 600);
    REQUIRE(size.second == 400);
}

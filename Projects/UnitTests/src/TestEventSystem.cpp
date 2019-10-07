#define CATCH_CONFIG_MAIN
#include <Catch.h>

#include <Engine/Event/EventManager.h>


// ---------------- Event testing ----------------
struct EventA : public ym::Event
{
	EventA(int a) : a(a) {}
	int a;
};

struct EventB : public ym::Event
{
	EventB(bool b) : b(b) {}
	bool b;
};

struct SubA
{
	SubA(std::string name, size_t level = 0) : name(name), level(level) {
		ym::EventManager::Get().Subscribe(this, &SubA::OnA, level);
	}

	bool OnA(EventA* a)
	{
		spdlog::info("(Lvl: {0:d}) {1:s} responded on A. a = {2:d}", level, name, a->a);
		return true;
	}

	std::string name;
	size_t level;
};

struct SubB
{
	SubB(std::string name, size_t level = 0) : name(name), level(level) {
		ym::EventManager::Get().Subscribe(this, &SubB::OnB, level);
	}

	bool OnB(EventB* a)
	{
		spdlog::info("(Lvl: {0:d}) {1:s} responded on B. b = {2:b}", level, name, a->b);
		return true;
	}
	std::string name;
	size_t level;
};

struct SubAB
{
	SubAB(std::string name, size_t level = 0) : name(name), level(level) {
		ym::EventManager::Get().Subscribe(this, &SubAB::OnA, level);
		ym::EventManager::Get().Subscribe(this, &SubAB::OnB, level);
	}

	bool OnA(EventA* a)
	{
		spdlog::info("(Lvl: {0:d}) {1:s} responded on A. a = {2:d}", level, name, a->a);
		return true;
	}

	bool OnB(EventB* a)
	{
		spdlog::info("(Lvl: {0:d}) {1:s} responded on B. b = {2:b}", level, name, a->b);
		return true;
	}
	std::string name;
	size_t level;
};

TEST_CASE("Event Tests", "[YamiEngine]")
{
	using namespace ym;
	SubAB subAB3("SubAB_3", 3);
	SubA subA3("SubA_3", 3);
	SubB subB3("SubB_3", 3);

	SubA subA2("SubA_2", 2);
	SubB subB2("SubB_2", 2);
	SubAB subAB2("SubAB_2", 2);

	SubA subA1("SubA_1", 1);
	SubB subB1("SubB_1", 1);
	SubAB subAB1("SubAB_1", 1);

	EventManager::Get().PrintInfo();

	EventManager::Get().Unsubscribe(&subAB2, &SubAB::OnB);

	EventManager::Get().Publish(&EventA(1), 1);
	EventManager::Get().Publish(&EventB(true));

	EventManager::Get().PrintInfo();
}

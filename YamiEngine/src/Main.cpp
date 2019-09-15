#ifdef YAMI_DEBUG
	#include <crtdbg.h>
#endif

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "Event/EventManager.h"

struct EventA : public Event
{
	EventA(int a) : a(a) {}
	int a;
};

struct EventB : public Event
{
	EventB(bool b) : b(b) {}
	bool b;
};

struct SubA
{
	SubA(std::string name, size_t level = 0) : name(name), level(level) {
		EventManager::Get().Subscribe(this, &SubA::OnA, level);
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
		EventManager::Get().Subscribe(this, &SubB::OnB, level);
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
		EventManager::Get().Subscribe(this, &SubAB::OnA, level);
		EventManager::Get().Subscribe(this, &SubAB::OnB, level);
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

int main()
{
#ifdef YAMI_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	spdlog::set_level(spdlog::level::debug);
#endif

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

	std::getchar();
	return 0;
}
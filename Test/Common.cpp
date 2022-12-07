#include "Common.h"

#include "gtest/gtest.h"

#include "Config.h"
#include "Core/Application.h"
#include "Logger/LoggerInternal.h"

// using namespace Core;

static std::unique_ptr<Application> g_TestApp = nullptr;

void createTestApplication()
{
	if (g_TestApp != nullptr)
	{
		// g_TestApp->finalize();
		g_TestApp = nullptr;
	}

	int argc = 1;
	char* argv[] = {(char*)"dummy"};
	INIT_LOGGER(argc, argv);

	g_TestApp = std::make_unique<Application>();

	g_TestApp->initWindow();
	g_TestApp->init();
	ASSERT_TRUE(g_TestApp->initI3T());
	g_TestApp->initModules();
}

void destroyTestApplication()
{
	/*
	ASSERT_TRUE(g_TestApp != nullptr);
	g_TestApp->finalize();

	g_TestApp = nullptr;
	 */
}

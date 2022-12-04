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

#ifdef I3T_RELEASE_STANDALONE // variant for standalone release
	Config::WORKING_DIRECTORY = "";
#else // special settings for usage in Visual Studio devenv
	Config::WORKING_DIRECTORY = I3T_PROJECT_ROOT;
#endif

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

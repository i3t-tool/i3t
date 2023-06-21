#include "Common.h"

#include "gtest/gtest.h"

#include "Config.h"
#include "Core/Application.h"
#include "Logger/Logger.h"
#include "Utils/Format.h"
#include "Utils/Math.h"

// using namespace Core;

static std::shared_ptr<Application> g_TestApp = nullptr;

void createTestApplication(bool fullInit)
{
	ASSERT_TRUE(g_TestApp == nullptr);

	int argc = 1;
	char* argv[] = {(char*) "dummy"};
	INIT_LOGGER(argc, argv);

	g_TestApp = std::make_shared<Application>();

	g_TestApp->initWindow();
	g_TestApp->init();
	if (fullInit)
	{
		ASSERT_TRUE(g_TestApp->initI3T());
	}
	g_TestApp->initModules();
}

void destroyTestApplication()
{
	ASSERT_TRUE(g_TestApp != nullptr);

	if (g_TestApp != nullptr)
	{
		g_TestApp->finalize();
		g_TestApp.reset();
	}
}

bool compare(const glm::mat4& lhs, const glm::mat4& rhs, float epsilon)
{
	auto result = Math::eq(lhs, rhs, epsilon);
	if (!result)
	{
		std::cerr << Utils::toString(lhs) << std::endl;
		std::cerr << "!=\n";
		std::cerr << Utils::toString(rhs) << std::endl;
	}

	return result;
}

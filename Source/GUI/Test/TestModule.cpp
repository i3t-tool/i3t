/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "GUI/Test/TestModule.h"

#include <cstdlib>

#include "imgui_te_context.h"
#include "imgui_te_engine.h"
#include "imgui_te_exporters.h"
#include "imgui_te_ui.h"

#include "Commands/ApplicationCommands.h"
#include "GUI/Test/TestWorkspace.h"

static void registerTests(ImGuiTestEngine* engine)
{
	auto* test = IM_REGISTER_TEST(engine, "demo_tests", "test1");
	test->GuiFunc = [](ImGuiTestContext* ctx) // Optionally provide a GUI function in addition to your application GUI
	{
		ImGui::Begin("Test Window", nullptr, ImGuiWindowFlags_NoSavedSettings);
		ImGui::Text("Hello, automation world");
		ImGui::Button("Click Me");
		if (ImGui::TreeNode("Node"))
		{
			static bool b = false;
			ImGui::Checkbox("Checkbox", &b);
			ImGui::TreePop();
		}
		ImGui::End();
	};
	test->TestFunc = [](ImGuiTestContext* ctx) // Generally provide a Test function which will drive the test.
	{
		ctx->SetRef("Test Window");
		ctx->ItemClick("Click Me");
		ctx->ItemOpen("Node"); // Optional as ItemCheck("Node/Checkbox") can do it automatically
		ctx->ItemCheck("Node/Checkbox");
		ctx->ItemUncheck("Node/Checkbox");
	};
}

ImGuiTest* TestModule::createTest(const char* group, const char* name)
{
	auto* test = IM_REGISTER_TEST(m_engine, group, name);
	if (m_autoRun)
	{
		ImGuiTestEngine_QueueTest(m_engine, test);
	}

	return test;
}

void TestModule::onInit()
{
	m_autoRun = std::getenv(I3T_TEST_AUTO_RUN) != nullptr;

	// Initialize the test engine
	m_engine = ImGuiTestEngine_CreateContext();
	ImGuiTestEngineIO& testIo = ImGuiTestEngine_GetIO(m_engine);
	testIo.ConfigVerboseLevel = ImGuiTestVerboseLevel_Info;
	testIo.ConfigVerboseLevelOnError = ImGuiTestVerboseLevel_Debug;
	testIo.ConfigSavedSettings = false;

	// Register your tests here
	registerTests(m_engine);
	registerWorkspaceTests(m_engine);

	if (m_autoRun)
	{
		ImGuiTestEngine_QueueTests(m_engine, ImGuiTestGroup_Unknown, "all");
	}

	ImGuiTestEngine_Start(m_engine, ImGui::GetCurrentContext());

	ImGuiTestEngine_InstallDefaultCrashHandler();
}

void TestModule::onBeginFrame()
{
	// Optionally: show test engine UI to browse/run test from the UI
	if (m_showTestWindow)
	{
		ImGuiTestEngine_ShowTestEngineWindows(m_engine, &m_showTestWindow);
	}
}

void TestModule::onUpdate(double deltaSeconds)
{
	if (m_autoRun && ImGuiTestEngine_IsTestQueueEmpty(m_engine))
	{
		static bool done = false;
		if (done)
		{
			return;
		}

		int countTested = 0;
		int countSuccess = 0;
		ImGuiTestEngine_GetResult(m_engine, countTested, countSuccess);
		ImGuiTestEngine_PrintResultSummary(m_engine);

		const auto exitCode = countTested != countSuccess ? 1 : 0;

		if (countTested != 0)
		{
			done = true;
			CloseCommand::dispatch(exitCode);
		}
	}
}

void TestModule::onEndFrame()
{
	// Call after your rendering.
	// This is mostly to support screen/video capturing features.
	ImGuiTestEngine_PostSwap(m_engine);
}

void TestModule::onClose()
{
	// May block until TestFunc thread/coroutine joins
	ImGuiTestEngine_Stop(m_engine);
	ImGuiTestEngine_DestroyContext(m_engine);
}

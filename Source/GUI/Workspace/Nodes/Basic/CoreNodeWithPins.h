/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Core/Nodes/GraphManager.h"

#include "CoreNode.h"
#include "CorePins.h"
#include "DIWNE/Core/Layout/VStack.h"

namespace Workspace
{
// TODO: Make this node part of DIWNE (BasicNodeWithPins or something)
class CoreNodeWithPins : public CoreNode
{
	using Super = CoreNode;

private:
	float m_minRightAlignOfRightPins{0};

protected:
	std::vector<Ptr<CorePin>> m_workspaceInputs;
	std::vector<Ptr<CorePin>> m_workspaceOutputs;
	bool m_showDataOnPins; //< default true, false for Camera and Sequence - they do not show data on their output pins

	DIWNE::VStack outputPinsVstack{diwne, &m_right};

public:
	CoreNodeWithPins(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase, bool showDataOnPins = true);

	/**
	 * \brief get vector of input pins
	 * \return m_workspaceInputs
	 */
	const std::vector<Ptr<CorePin>>& getInputs() const
	{
		return m_workspaceInputs;
	};
	/**
	 * \brief get vector of output pins
	 * \return m_workspaceOutputs
	 */
	const std::vector<Ptr<CorePin>>& getOutputs() const
	{
		return m_workspaceOutputs;
	};
	virtual std::vector<Ptr<CorePin>> getOutputsToShow() const
	{
		return getOutputs();
	};

	void leftContent(DIWNE::DrawInfo& context) override;
	void rightContent(DIWNE::DrawInfo& context) override;

	virtual void drawInputPins(DIWNE::DrawInfo& context);
	virtual void drawOutputPins(DIWNE::DrawInfo& context);

	void onDestroy(bool logEvent) override;

	void translate(const ImVec2& vec) override;
};
} // namespace Workspace

/* \todo maybe create from this function class "WithPins" and inherit other
 * class from "WithPins" */
// extern void loadWorkspacePinsFromCorePins(WorkspaceNodeWithCoreData&
// workspaceNode, Core::Node::ConstPinListRef coreInputPins,
// Core::Node::ConstPinListRef coreOutputPins,
// std::vector<Ptr<WorkspaceCorePin>> & workspaceInputPins,
// std::vector<Ptr<WorkspaceCorePin>> & workspaceOutputPins);

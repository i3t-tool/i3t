#pragma once

#include "Core/Defs.h"
#include "Core/Nodes/GraphManager.h"
#include "Core/Nodes/Id.h"
#include "State/NodeVisitor.h"
#include "State/Stateful.h"

std::optional<Ptr<GuiNode>> findNodeById(const std::vector<Ptr<GuiNode>>& nodes, Core::ID id);

Memento copyNodes(const std::vector<Ptr<GuiNode>>& nodes);

void pasteNodes(const Memento& memento);

//----------------------------------------------------------------------------//

class Tracking
{
public:
	Tracking(Ptr<WorkspaceSequence> beginSequence);

	/**
	 * \see Core::Matrix::Tracker::setParam.
	 */
	void setParam(float value);

	float getParam() const;

private:
	Core::MatrixTracker m_internal;
};

/**
 * \file
 * \brief
 * \authors Martin Herich
 * \copyright Copyright (C) 2016-2024 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"

#include <string>

namespace Workspace
{
class ScriptingNode : public CoreNodeWithPins
{
public:
	ScriptingNode(DIWNE::Diwne& diwne);
	ScriptingNode(DIWNE::Diwne& diwne, const std::string& script);

	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<ScriptingNode>(shared_from_this()));
	}

	void popupContent() override;
	int maxLengthOfData() override;
	void drawMenuLevelOfDetail() override;

	void reloadScript();
};
} // namespace Workspace

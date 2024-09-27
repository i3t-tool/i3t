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

#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"

namespace Workspace
{
template <Core::EOperatorType T>
class Operator : public CoreNodeWithPins
{
public:
	Operator(DIWNE::Diwne& diwne) : CoreNodeWithPins(diwne, Core::Builder::createOperator<T>())
	{
		// To avoid rounding-confusion, the operators with quaternion output should have more decimal places to
		// distinguish small changes near zero or near one
		//
		// Example: (0.9986, 0.0, 0.0524, 0.0) is a normalized quaternion
		//          (1.0, 0.0, 0.1, 0.0)       is its representation rounded to 1 decimal digit
		//
		//          Both return length equal to 1.0, but the rounded is clearly not normalized.
		switch (const Core::EOperatorType t = T)
		{
		case Core::EOperatorType::QuatToQuat:
		case Core::EOperatorType::AngleAxisToQuat:
		case Core::EOperatorType::ConjQuat:
		case Core::EOperatorType::EulerToQuat:
		case Core::EOperatorType::FloatMulQuat:
		case Core::EOperatorType::FloatVecToQuat:
		case Core::EOperatorType::FloatsToQuat:
		case Core::EOperatorType::MatrixToQuat:
		case Core::EOperatorType::NormalizeQuat:
		case Core::EOperatorType::QuatToFloats:
		case Core::EOperatorType::QuatInverse:
		case Core::EOperatorType::QuatLength:
		case Core::EOperatorType::QuatMulQuat:
		case Core::EOperatorType::QuatVecConjQuat:
		case Core::EOperatorType::VecVecToQuat:
			CoreNode::setNumberOfVisibleDecimal(I3T::getTheme().get(ESize::Default_VisibleQuaternionPrecision));
			break;
		default:
		    // leave the default precision;
		    ;
		}

		// if (Core::EOperatorType::QuatToQuat == T)
		//{
		//	WorkspaceNodeWithCoreData::setNumberOfVisibleDecimal(4);
		// }

		updateDataItemsWidth();
	}

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<CoreNodeWithPins>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	virtual bool beforeContent() override
	{
		/* whole node background */
		diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_bottomRectDiwne.Max, I3T::getTheme().get(EColor::NodeBgOperator),
		                         I3T::getSize(ESize::Nodes_Operators_Rounding), ImDrawFlags_RoundCornersAll);
		return false;
	}

	virtual bool topContent() override
	{
		diwne.AddRectFilledDiwne(m_topRectDiwne.Min, m_topRectDiwne.Max,
		                         I3T::getTheme().get(EColor::NodeHeaderOperator),
		                         I3T::getSize(ESize::Nodes_Operators_Rounding), ImDrawFlags_RoundCornersTop);

		return CoreNode::topContent();
	}

	virtual bool inline middleContent() override
	{
		return false;
	}

	void drawMenuLevelOfDetail() override
	{
		drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
		                              {LevelOfDetail::Full, LevelOfDetail::Label});
	}

	int maxLengthOfData() override
	{
		int maxLen = 0;
		for (auto const& pin : m_workspaceOutputs)
		{
			maxLen = std::max(maxLen, std::dynamic_pointer_cast<DataOutPin>(pin)->maxLengthOfData());
		}
		return maxLen;
	}
};

class AngleAxisToQuatOperator : public Operator<Core::EOperatorType::AngleAxisToQuat>
{
public:
	bool m_halfAngle; ///< true == pin index 1, false == pin index 0 (full angle)
	AngleAxisToQuatOperator(DIWNE::Diwne& diwne)
	    : Operator<Core::EOperatorType::AngleAxisToQuat>(diwne),
	      m_halfAngle(false) /* true == pin index 1, false == pin index 0 */
	{}

	bool leftContent() override
	{
		bool interaction_happen = false;

		interaction_happen |= ImGui::Checkbox("/2", &m_halfAngle);

		if (interaction_happen) /* mode changed */
		{
			if (m_workspaceInputs.at(m_halfAngle ? 0 : 1)->isConnected()) /* previous mode pin is connected */
			{
				/* connect visible pin and unplug hidden one */
				m_workspaceInputs.at(m_halfAngle ? 1 : 0)
				    .get()
				    ->plug(m_workspaceInputs.at(m_halfAngle ? 0 : 1)->getLink().getStartPin());
				m_workspaceInputs.at(m_halfAngle ? 0 : 1)->unplug();
			}
		}

		interaction_happen |= m_workspaceInputs.at(m_halfAngle ? 1 : 0)->drawDiwne();
		interaction_happen |= m_workspaceInputs.at(2)->drawDiwne();

		return interaction_happen;
	}
};
} // namespace Workspace
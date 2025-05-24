/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "DIWNE/Core/Style/StyleOverride.h"
#include "GUI/Workspace/Nodes/Basic/CoreNodeWithPins.h"
#include "GUI/Workspace/Nodes/Basic/DataRenderer.h"

namespace Workspace
{
template <Core::EOperatorType T>
class Operator : public CoreNodeWithPins
{
public:
	explicit Operator(DIWNE::NodeEditor& diwne) : CoreNodeWithPins(diwne, Core::Builder::createOperator<T>())
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

		setStyleOverride(&I3T::getTheme().m_operatorStyle);
	}

	//===-- Double dispatch
	//---------------------------------------------------===//
	void accept(NodeVisitor& visitor) override
	{
		visitor.visit(std::static_pointer_cast<CoreNodeWithPins>(shared_from_this()));
	}
	//===----------------------------------------------------------------------===//

	// TODO: Uncomment
	//	virtual bool beforeContent() override
	//	{
	//		// TODO: Refactor, remove method and merge into content / begin
	//		/* whole node background */
	//		diwne.canvas().AddRectFilledDiwne(m_top.getMin(), m_bottom.getMax(),
	// I3T::getTheme().get(EColor::NodeBgOperator), I3T::getSize(ESize::Nodes_Operators_Rounding),
	// ImDrawFlags_RoundCornersAll); 		return false;
	//	}

	void centerContent(DIWNE::DrawInfo& context) override
	{
		// Why is this empty?
		// Because operators are essentially only made up of pins
	}

	void drawMenuLevelOfDetail() override
	{
		drawMenuLevelOfDetail_builder(std::dynamic_pointer_cast<CoreNode>(shared_from_this()),
		                              {LevelOfDetail::Full, LevelOfDetail::Label});
	}

	int maxLengthOfData() override
	{
		int maxLen = 0;
		for (auto const& pin : m_rightPins)
		{
			maxLen = std::max(maxLen, pin->maxLengthOfData());
		}
		return maxLen;
	}
};

class AngleAxisToQuatOperator : public Operator<Core::EOperatorType::AngleAxisToQuat>
{
public:
	bool m_halfAngle; ///< true == pin index 1, false == pin index 0 (full angle)
	AngleAxisToQuatOperator(DIWNE::NodeEditor& diwne)
	    : Operator<Core::EOperatorType::AngleAxisToQuat>(diwne),
	      m_halfAngle(false) /* true == pin index 1, false == pin index 0 */
	{}

	void leftContent(DIWNE::DrawInfo& context) override
	{
		if (ImGui::Checkbox("/2", &m_halfAngle)) /* mode changed */
		{
			context.update(true, true);
			if (m_workspaceInputs.at(m_halfAngle ? 0 : 1)->isConnected()) /* previous mode pin is connected */
			{
				/* connect visible pin and unplug hidden one */
				Ptr<DIWNE::Pin> visiblePin = m_workspaceInputs.at(m_halfAngle ? 1 : 0);
				Ptr<DIWNE::Pin> hiddenPin = m_workspaceInputs.at(m_halfAngle ? 0 : 1);
				visiblePin->plugLink(hiddenPin->getLink()->getStartPin(), hiddenPin->getLink(), false);
			}
		}

		m_workspaceInputs.at(m_halfAngle ? 1 : 0)->drawDiwne(context, m_drawMode);
		m_workspaceInputs.at(2)->drawDiwne(context, m_drawMode);
	}
};
} // namespace Workspace
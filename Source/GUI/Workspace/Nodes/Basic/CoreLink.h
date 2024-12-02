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

#include "DIWNE/diwne_include.h"

namespace Workspace
{

class CoreOutPin;
class CoreInPin;

///*! \class WorkspaceCoreLink
class CoreLink : public DIWNE::Link
{
protected:
	CoreOutPin* m_startPin;
	CoreInPin* const m_endPin;

public:
	CoreLink(DIWNE::NodeEditor& diwne, CoreInPin* endPin);

	CoreOutPin* getStartPin() const
	{
		return m_startPin;
	};
	CoreInPin* const getEndPin() const
	{
		return m_endPin;
	};
	void setStartPin(CoreOutPin* startPin)
	{
		m_startPin = startPin;
	};
	// TODO: setEndPin is missing
	void unplug();

	void popupContent(DIWNE::DrawInfo& context) override;
	void initialize(DIWNE::DrawInfo& context) override;

	void updateEndpoints() override;
	void updateControlPointsOffsets();
};
} // namespace Workspace
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

#include "DIWNE/diwne_include.h"

namespace Core
{
enum class EValueType;
}
namespace Workspace
{

class CorePin;

class CoreLink : public DIWNE::Link
{
public:
	CoreLink(DIWNE::NodeEditor& diwne);

	void initialize(DIWNE::DrawInfo& context) override;
	void content(DIWNE::DrawInfo& context) override;

	void popupContent(DIWNE::DrawInfo& context) override;

	bool allowSelectOnClick(const DIWNE::DrawInfo& context) const override;

	void updateControlPoints() override;

protected:
	Core::EValueType m_coreType;
};
} // namespace Workspace
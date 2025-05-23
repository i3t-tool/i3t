/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "StyleOverride.h"

#include "StyleBase.h"

namespace DIWNE
{

const ImVec4& StyleOverride::color(Style::Var key) const
{
	return get<ImVec4>(key);
}
ImVec2 StyleOverride::size(Style::Var key) const
{
	const StyleBase::StyleEntry* entry = nullptr;
	ImVec2 ret = getImpl<ImVec2>(key, &entry);
	if (entry->dpiScaled)
		ret *= editor->styleBase().dpiScale;
	return ret;
}
float StyleOverride::decimal(Style::Var key) const
{
	const StyleBase::StyleEntry* entry = nullptr;
	float ret = getImpl<float>(key, &entry);
	if (entry->dpiScaled)
		ret *= editor->styleBase().dpiScale;
	return ret;
}
int StyleOverride::integer(Style::Var key) const
{
	return get<int>(key);
}
bool StyleOverride::boolean(Style::Var key) const
{
	return get<bool>(key);
}

} // namespace DIWNE
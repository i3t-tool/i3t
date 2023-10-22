/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#include "Scene.h"

#include "Utils/JSON.h"

namespace State
{
bool isReadOnly(const fs::path& scene)
{
	rapidjson::Document doc;
	if (!JSON::parse(scene, doc, I3T_SCENE_SCHEMA))
	{
		return false;
	}

	if (!doc.HasMember("readOnly"))
	{
		return false;
	}

	return doc["readOnly"].GetBool();
}
} // namespace State

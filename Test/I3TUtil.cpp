/**
 * \file I3TUtil.cpp
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
#include "I3TUtil.h"

#include "I3T.h"

std::unique_ptr<I3TApplication> initI3T()
{
	std::unique_ptr<I3TApplication> app = std::make_unique<I3TApplication>();
	app->init();
	return app;
}

void ApplicationTest::SetUp()
{
	app = std::move(initI3T());
}

void ApplicationTest::TearDown()
{
	app->close();
}

/**
 * \file
 * \brief Declares the common time function. Previously common.h.
 * \authors Michal Folta, Dan Rakušan
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include <string>

namespace TimeUtils
{

// time measurement
long GetTime();
long GetRealTime();
float TimeDiff(long time1, long time2);

long int minutesSinceEpoch();
long int stringToMinutesSinceEpoch(std::string str);
std::string minutesSinceEpochToString(long int minutes);

} // namespace TimeUtils
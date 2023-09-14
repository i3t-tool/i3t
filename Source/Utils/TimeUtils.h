#pragma once

/**
 * \file	TimeUtils.h
 *
 * Declares the common time function. Previously common.h.
 *
 * \author  Michal Folta, Dan Rakušan
 * \date    2014/11/16
 */

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
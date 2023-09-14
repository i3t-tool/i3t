#include "TimeUtils.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <sys/timeb.h>

namespace TimeUtils
{

#define TIME_STRING_FORMAT "%Y-%m-%d %H:%M:%S"

/**
 * \fn	long GetTime()
 *
 * \brief	Gets the time.  Synonym of GetRealTime()
 *
 * \return	The time.
 */

long GetTime()
{
	// static struct _timeb mtime;
	static struct timeb mtime;
	//_ftime64_s(&mtime);
	ftime(&mtime);
	return (long) (1000 * (1000 * mtime.time + mtime.millitm));
}

/**
 * \fn	long GetRealTime()
 *
 * \brief	Gets real time. Synonym of GetTime()
 *
 * \return	The real time.
 */

long GetRealTime()
{
	// static struct _timeb mtime;
	static struct timeb mtime;

	//_ftime64_s(&mtime);
	ftime(&mtime);
	return (long) (1000 * (1000 * mtime.time + mtime.millitm));
}

/**
 * \fn	float TimeDiff(long time1,long time2)
 *
 * \brief	Time difference in seconds
 *
 * \param	time1	The first time in [ms].
 * \param	time2	The second time in [ms].
 *
 * \return	A float.
 */

float TimeDiff(long time1, long time2)
{
	// in ms
	const float clk = 1.0e-3f; // ticks per second
	long t = time2 - time1;
	return ((t < 0) ? -t : t) * clk;
}

long int minutesSinceEpoch()
{
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::minutes>(tp.time_since_epoch()).count();
}

long int stringToMinutesSinceEpoch(std::string str)
{
	std::tm tm = {};
	std::stringstream ss(str);
	ss >> std::get_time(&tm, TIME_STRING_FORMAT);
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(mktime(&tm));
	return std::chrono::duration_cast<std::chrono::minutes>(tp.time_since_epoch()).count();
}

std::string minutesSinceEpochToString(long int minutes)
{
	auto epoch = std::chrono::time_point<std::chrono::system_clock>{};
	epoch += std::chrono::minutes(minutes);
	auto epoch_time_t = std::chrono::system_clock::to_time_t(epoch);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&epoch_time_t), TIME_STRING_FORMAT);
	return ss.str();
}
} // namespace TimeUtils
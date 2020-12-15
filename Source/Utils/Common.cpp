#include "common.h"
#include <sys/timeb.h>

/**
 * \fn	long GetTime()
 *
 * \brief	Gets the time.  Synonym of GetRealTime()
 *
 * \return	The time.
 */

long GetTime()
{
  static struct _timeb mtime;
  _ftime64_s(&mtime);
  return (long)(1000 * (1000 * mtime.time + mtime.millitm));
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
  static struct _timeb mtime;
  _ftime64_s(&mtime);
  return (long)(1000 * (1000 * mtime.time + mtime.millitm));
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

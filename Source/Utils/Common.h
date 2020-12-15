//---------------------------------------------------------------------------
/**
 * \file	common.h
 *
 * Declares the common time function
 * 
 * \author  Michal Folta, CTU Prague
 * \date    2014/11/16
 */
//---------------------------------------------------------------------------


#ifndef _COMMON_H_
#define _COMMON_H_

#define STATISTIC

// time measurement
long GetTime();
long GetRealTime();
float TimeDiff(long time1, long time2);

#endif

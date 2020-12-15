//---------------------------------------------------------------------------
/**
 * \file	tMshReader.h
 *
 * Declares .tmsh file reader class.
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague		  
 */
//---------------------------------------------------------------------------

#ifndef _T_MSH_READER_H_
#define _T_MSH_READER_H_

#include <string>


class Geometry;

/** .tmsh file reader. */
class TMshReader
{
public:

  static Geometry* geometryFromTMsh(std::string filename);

  static Geometry* geometryFromTMsh(std::string filename, float scale);
};


#endif

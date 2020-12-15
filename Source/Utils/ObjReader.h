//---------------------------------------------------------------------------
/**
 * \file	objReader.h
 *
 * Declares the object reader class (for reading of .obj files)
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _TABSPACE_H_
#define _TABSPACE_H_

class Geometry;

class ObjReader
{
public:
  static Geometry* geometryFromOBJ(const char* filename);
  static Geometry* geometryFromOBJ(const char* filename, float scale);
};

#endif

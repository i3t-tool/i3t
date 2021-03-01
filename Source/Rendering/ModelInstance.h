//---------------------------------------------------------------------------
/**
 * \file	modelInstance.h
 *
 * Header for representing instance of Model
 * \date  2014/11/16
 * \author Michal Folta, CTU Prague
 */
//---------------------------------------------------------------------------

#ifndef _MODEL_INSTANCE_H_
#define _MODEL_INSTANCE_H_

class Shader;

#include "Geometry.h"
#include "Rendering/Material.h"
#include "Transformable.h"

class ModelInstance : public Transformable
{
protected:
	bool visible;

	bool alphaFlag; ///<

	Geometry* geometry; ///< model geometry (vertice, OpenGL buffers, AABB, ...)

public:
	Material* material;

	ModelInstance(Geometry* _geometry, Material* _material)
			: geometry(_geometry), material(_material), alphaFlag(false), visible(true)
	{
	}

	virtual ~ModelInstance() {}

	Geometry* getGeometry() const { return geometry; }

	/**
	 * \brief \todo NOT USED. Set material alphaFlag
	 * \param val should set the material alphaFlag to this val
	 */
	void setAlphaFlag(const bool val) { alphaFlag = val; }

	bool getAlphaFlag() const { return alphaFlag; }

	void setVisible(const bool val) { visible = val; }

	bool isVisible() const { return visible; }

	virtual void draw() {}
};

#endif

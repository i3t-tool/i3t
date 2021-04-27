#pragma once
//----------------
/**
 * \file Component.h
 * \author Daniel Gruncl
 * \brief Component is added to GameObject
 * Abstract class.
 */
//---------------
#include <typeinfo>

#include "GameObject.h"
#include "pgr.h"

class GameObject;

class Component
{
public:
  virtual void start();
  virtual void update();
  virtual void render(glm::mat4* parent, bool renderTransparent);
  virtual const char* getComponentType();

  GameObject* m_gameObject;
  bool m_isActive;

protected:
  static const char* s_type;
  const char* m_type;
  Component();
};
#pragma once
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

  GameObject* owner;
  bool isActive;

protected:
  static const char* typeStatic;
  const char* type;
  Component();
};
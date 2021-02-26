#pragma once
#include "../Component.h"

class Renderer : public Component
{
public:
  Renderer(unsigned int flags = 0);
  // void start();
  void render(glm::mat4* parent, bool renderTransparent);
  static const char* componentType() { return Renderer::typeStatic; };
  const char* getComponentType() { return Renderer::typeStatic; };

  bool isTransparent = false;
  bool useStencil = false;
  bool drawLines = false;
  unsigned char stencil = 0;
  static const unsigned int IS_TRANSPARENT = 1 << 0, USE_STENCIL = 1 << 1, DRAW_LINES = 1 << 2;
  // static Component*stencilRef[256];
  // static int registerStencil(Component*req);
private:
  static const char* typeStatic;
};
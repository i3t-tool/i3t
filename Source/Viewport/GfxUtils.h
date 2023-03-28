#pragma once

#include <glm/glm.hpp>

namespace Vp
{
namespace GfxUtils
{
void extractZNearZFar(glm::mat4 projectionMatrix, float& zNear, float& zFar);
}

} // namespace Vp

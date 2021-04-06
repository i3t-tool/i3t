#include "NodeData.h"

using namespace Core;

bool Transform::cmp(const Transform::DataMap& map, const glm::mat4& mat)
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      auto val = map[4 * i + j];
      switch (map[4 * i + j])
      {
      case 0:
        if (!Math::eq(mat[i][j], 0.0f) || !Math::eq(mat[i][j], -0.0f))
        {
          return false;
        }
				break;
      case 255:
        if (!Math::eq(mat[i][j], 1.0f))
        {
          return false;
        }
      }
    }
  }

  return true;
}

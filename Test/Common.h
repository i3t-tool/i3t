#pragma once

#include "glm/matrix.hpp"

bool compare(const glm::mat4& lhs, const glm::mat4& rhs, float epsilon = 0.001f);

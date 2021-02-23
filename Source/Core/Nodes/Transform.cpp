#include "Transform.h"

#include "glm/gtx/rotate_vector.hpp"

#include "Logger/Logger.h"

using namespace Core;

ValueSetResult Core::Scale::setValue(const glm::vec3& vec)
{
  if (m_currentMap == Transform::g_UniformScale)
  {
    if (Math::areElementsSame(vec))
      m_internalData[0].setValue(glm::scale(vec));
    else
      return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Given vector does not have all three values same."};
  }
  else
  {
    m_internalData[0].setValue(glm::scale(vec));
  }

  return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Scale::setValue(const glm::mat4& mat)
{
  if (m_currentMap == Transform::g_UniformScale)
  {
    if (Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]))
      m_internalData[0].setValue(mat);
    else
      return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Given matrix does not represent uniform scale."};
  }
  else if (m_currentMap == Transform::g_Scale)
  {
    if (Transform::cmp(m_currentMap, mat))
    {
      m_internalData[0].setValue(mat);
    }
    else
    {
      return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Given matrix does not represent scale."};
    }
  }
  else if (m_currentMap == Transform::g_Free)
  {
    // Free transformation is set.
    getData().getMat4Ref() = mat;
  }

  return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Scale::setValue(float val, glm::ivec2 coords)
{
  if (m_currentMap == Transform::g_Free)
  {
    // Free transformation is set.
    getData().getMat4Ref()[coords.x][coords.y];
  }

  if (!coordsAreValid(coords, m_currentMap))
  {
    return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};;
  }

  if (m_currentMap == Transform::g_UniformScale)
  {
    getInternalData().setValue(glm::scale(glm::vec3(val)));
  }

  if (m_currentMap == Transform::g_Scale)
  {
    getData().getMat4Ref()[coords.x][coords.y];
  }

  return ValueSetResult{ValueSetResult::Status::Ok};
}

void Scale::reset()
{
  setDataMap(m_initialMap);
  getInternalData().setValue(glm::scale(m_initialScale));
}

void Scale::setDataMap(const Transform::DataMap& map)
{
  Debug::Assert(map == Transform::g_Free || map == Transform::g_Scale || map == Transform::g_UniformScale);
  NodeBase::setDataMap(map);
}

void Scale::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::scale(m_inputs[0].getStorage().getVec3()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

//===-- Euler rotation around X axis --------------------------------------===//
ValueSetResult EulerRotX::setValue(float val)
{
  getData().getVec3Ref() = glm::rotateX(glm::vec3(1.0f, 0.0f, 0.0f), val);

  return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult EulerRotX::setValue(const glm::mat4& mat)
{
  if (m_currentMap == Transform::g_EulerX)
  {
    if (Transform::isMatValid(m_currentMap, mat))
    {
      /// \todo Rotation matrix values validation.
      Debug::Assert(false, "Not implemented yet.");
    }
  }
  else if (m_currentMap == Transform::g_Free)
  {
    getData().setValue(mat);
  }

  return ValueSetResult{ValueSetResult::Status::Ok};
}

void EulerRotX::reset()
{
  setDataMap(m_initialMap);
  auto result = setValue(m_initialRot);
  if (result.status != ValueSetResult::Status::Ok)
  {
    Log::fatal("Could not reset values at EulerRotX#{}", getId());
  }
}

void EulerRotX::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    float angle;

    angle = m_inputs[0].getStorage().getFloat();

    m_internalData[0].setValue(glm::eulerAngleX(angle));
  }
  else
  {
    m_internalData[0].setValue(glm::eulerAngleX(0.0f));
  }
}

ValueSetResult EulerRotX::setValue(float val, glm::ivec2 coords)
{
  if (!coordsAreValid(coords, m_currentMap))
  {
      ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
  }

  if (!Math::withinInterval(val, -1.0f, 1.0f))
  {
    return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Value must be within [-1.0, 1.0] interval."};
  }

  auto& mat = getData().getMat4Ref();

  if (coords == glm::ivec2(1, 2))
  {
    // -sin(T)
    mat[1][2] = val;
    mat[2][1] = -val;

    auto cos = sqrt(1.0f - (val * val));
    mat[1][1] = cos;
    mat[2][2] = cos;
  }
  else if (coords == glm::ivec2(1, 1) || coords == glm::ivec2(2, 2))
  {
    // cos(T)
    mat[1][1] = val;
    mat[2][2] = val;

    auto sin = sqrt(1.0f - (val * val));
    mat[1][2] = sin;
    mat[2][1] = -sin;
  }
  else if (coords == glm::ivec2(2, 1))
  {
    // sin(T)
    mat[2][1] = val;
    mat[1][2] = -val;

    auto cos = sqrt(1.0f - (val * val));
    mat[1][1] = cos;
    mat[2][2] = cos;
  }

  return ValueSetResult{ValueSetResult::Status::Ok};
}

//===-- Euler rotation around Y axis --------------------------------------===//

ValueSetResult EulerRotY::setValue(float val)
{
  return NodeBase::setValue(glm::rotateY(glm::vec3(0.0f, 1.0f, 0.0f), val));
}

ValueSetResult EulerRotY::setValue(const glm::mat4&)
{
  Debug::Assert(false, "Not implemented yet.");
  return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Unsupported yet."};
}

void EulerRotY::reset()
{
  setDataMap(m_initialMap);
  auto result = setValue(m_initialRot);
  if (result.status != ValueSetResult::Status::Ok)
  {
    Log::fatal("Could not reset values at EulerRotY#{}.", getId());
  }
}

void EulerRotY::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    float angle;

    angle = m_inputs[0].getStorage().getFloat();

    m_internalData[0].setValue(glm::eulerAngleY(angle));
  }
  else
  {
    m_internalData[0].setValue(glm::eulerAngleY(0.0f));
  }
}

//===-- Euler rotation around Z axis --------------------------------------===//

ValueSetResult EulerRotZ::setValue(float val)
{
  getData().setValue(glm::rotateZ(glm::vec3(0.0f, 0.0f, 1.0f), val));

  return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult EulerRotZ::setValue(const glm::mat4&)
{
  Debug::Assert(false, "Not implemented yet.");
  return ValueSetResult{ValueSetResult::Status::Ok};
}

void EulerRotZ::reset()
{
  setDataMap(m_initialMap);
  auto result = setValue(m_initialRot);
  if (result.status != ValueSetResult::Status::Ok)
  {
    Log::fatal("Could not reset values at EulerRotX#{}", getId());
  }
}

void EulerRotZ::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    float angle;

    angle = m_inputs[0].getStorage().getFloat();

    m_internalData[0].setValue(glm::eulerAngleZ(angle));
  }
  else
  {
    m_internalData[0].setValue(glm::eulerAngleZ(0.0f));
  }
}

//===-- Euler rotation around Z axis --------------------------------------===//

ValueSetResult Core::Translation::setValue(const glm::vec3& vec)
{
  getData().setValue(glm::translate(vec));

  return ValueSetResult{ValueSetResult::Status::Ok};
}

ValueSetResult Translation::setValue(const glm::mat4& mat)
{
  if (m_currentMap == Transform::g_Translate)
  {
    if (Transform::isMatValid(m_currentMap, mat))
    {
      getData().setValue(mat);
    }
    else
    {
      return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set given matrix, because it not represents translation."};
    }
  }
  else if (m_currentMap == Transform::g_Free)
  {
    getData().setValue(mat);
  }

  return ValueSetResult{ValueSetResult::Status::Ok};
}

void Translation::reset()
{
  setDataMap(m_initialMap);
  auto result = setValue(m_initialTrans);
  if (result.status != ValueSetResult::Status::Ok)
  {
    Log::fatal("Could not reset values at EulerRotX#{}", getId());
  }
}

void Translation::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::translate(m_inputs[0].getStorage().getVec3()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4(1.0f));
  }
}

ValueSetResult Translation::setValue(float val, glm::ivec2 coords)
{
  if (!coordsAreValid(coords, m_currentMap))
  {
    return ValueSetResult{ValueSetResult::Status::Err_ConstraintViolation, "Cannot set value on given coordinates."};
  }
  getData().getMat4Ref()[coords.x][coords.y] = val;
  return ValueSetResult{ValueSetResult::Status::Ok};
}

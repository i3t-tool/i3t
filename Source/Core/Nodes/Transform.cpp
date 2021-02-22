#include "Transform.h"

#include "glm/gtx/rotate_vector.hpp"

using namespace Core;

EValueSetResult Core::Scale::setValue(const glm::vec3& vec)
{
  if (m_currentMap == Transform::g_UniformScale)
  {
    if (Math::areElementsSame(vec))
      m_internalData[0].setValue(glm::scale(vec));
    else
      return EValueSetResult::Err_ConstraintViolation;
  }
  else
  {
    m_internalData[0].setValue(glm::scale(vec));
  }

  return EValueSetResult::Ok;
}

EValueSetResult Scale::setValue(const glm::mat4& mat)
{
  auto result = EValueSetResult::Ok;

  if (m_currentMap == Transform::g_UniformScale)
  {
    if (Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]))
      m_internalData[0].setValue(mat);
    else
      result = EValueSetResult::Err_ConstraintViolation;
  }
  else if (m_currentMap == Transform::g_Scale)
  {
    if (Transform::cmp(m_currentMap, mat))
    {
      m_internalData[0].setValue(mat);
    }
    else
    {
      result = EValueSetResult::Err_ConstraintViolation;
    }
  }
  else if (m_currentMap == Transform::g_Free)
  {
    // Free transformation is set.
    NodeBase::setValue(mat);
  }

  return result;
}

EValueSetResult Scale::setValue(float val, glm::ivec2 coords)
{
  if (m_currentMap == Transform::g_Free)
  {
    // Free transformation is set.
    getData().getMat4Ref()[coords.x][coords.y];
  }

  if (coordsAreValid(coords, m_currentMap))
  {
    return EValueSetResult::Err_ConstraintViolation;
  }

  if (m_currentMap == Transform::g_UniformScale)
  {
    getInternalData().setValue(glm::scale(glm::vec3(val)));
  }

  if (m_currentMap == Transform::g_Scale)
  {
    getData().getMat4Ref()[coords.x][coords.y];
  }

  return EValueSetResult::Ok;
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
EValueSetResult EulerRotX::setValue(float val)
{
  NodeBase::setValue(glm::rotateX(glm::vec3(1.0f, 0.0f, 0.0f), val));

  return EValueSetResult::Ok;
}

EValueSetResult EulerRotX::setValue(const glm::mat4& mat)
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

  return EValueSetResult::Ok;
}

void EulerRotX::reset()
{
  setDataMap(m_initialMap);
  setValue(m_initialRot);
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

EValueSetResult EulerRotX::setValue(float val, glm::ivec2 coords)
{
  if (!coordsAreValid(coords, m_currentMap))
  {
    return EValueSetResult::Err_ConstraintViolation;
  }

  if (!Math::withinInterval(val, -1.0f, 1.0f))
  {
    return EValueSetResult::Err_ConstraintViolation;
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

  return EValueSetResult::Ok;
}

//===-- Euler rotation around Y axis --------------------------------------===//

EValueSetResult EulerRotY::setValue(float val)
{
  return NodeBase::setValue(glm::rotateY(glm::vec3(0.0f, 1.0f, 0.0f), val));
}

EValueSetResult EulerRotY::setValue(const glm::mat4&)
{
  Debug::Assert(false, "Not implemented yet.");
  return EValueSetResult::Ok;
}

void EulerRotY::reset()
{
  setDataMap(m_initialMap);
  setValue(m_initialRot);
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

EValueSetResult EulerRotZ::setValue(float val)
{
  getData().setValue(glm::rotateZ(glm::vec3(0.0f, 0.0f, 1.0f), val));

  return EValueSetResult::Ok;
}

EValueSetResult EulerRotZ::setValue(const glm::mat4&)
{
  Debug::Assert(false, "Not implemented yet.");
  return EValueSetResult::Ok;
}

void EulerRotZ::reset()
{
  setDataMap(m_initialMap);
  setValue(m_initialRot);
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

EValueSetResult Core::Translation::setValue(const glm::vec3& vec)
{
  getData().setValue(glm::translate(vec));

  return EValueSetResult::Ok;
}

EValueSetResult Translation::setValue(const glm::mat4& mat)
{
  if (m_currentMap == Transform::g_Translate)
  {
    if (Transform::isMatValid(m_currentMap, mat))
    {
      getData().setValue(mat);
    }
    else
    {
      return EValueSetResult::Err_ConstraintViolation;
    }
  }
  else if (m_currentMap == Transform::g_Free)
  {
    getData().setValue(mat);
  }

  return EValueSetResult::Ok;
}

void Core::Translation::reset()
{
  setDataMap(m_initialMap);
  setValue(m_initialTrans);
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

EValueSetResult Translation::setValue(float val, glm::ivec2 coords)
{
  if (coordsAreValid(coords, m_currentMap))
  {
    getData().getMat4Ref()[coords.x][coords.y] = val;
    return EValueSetResult::Ok;
  }
  else
  {
    return EValueSetResult::Err_ConstraintViolation;
  }
}

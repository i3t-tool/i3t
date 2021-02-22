#include "Transform.h"

#include "glm/gtx/rotate_vector.hpp"

#include "Utils/Math.h"

using namespace Core;

EValueSetResult Core::Scale::setValue(const glm::vec3& vec)
{
  if (eq(m_currentMap, g_UniformScale))
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

  if (eq(m_currentMap, g_UniformScale))
  {
    if (Math::eq(mat[0][0], mat[1][1]) && Math::eq(mat[1][1], mat[2][2]))
      m_internalData[0].setValue(mat);
    else
      result = EValueSetResult::Err_ConstraintViolation;
  }
  else if (eq(m_currentMap, g_Scale))
  {
    if (cmp(m_currentMap, mat))
    {
      m_internalData[0].setValue(mat);
    }
    else
    {
      result = EValueSetResult::Err_ConstraintViolation;
    }
  }
  else if (eq(m_currentMap, g_Free))
  {
    // Free transformation is set.
    NodeBase::setValue(mat);
  }

  return result;
}

void Scale::reset()
{
  setDataMap(m_initialMap);
  getInternalData().setValue(glm::scale(m_initialScale));
}

void Scale::setDataMap(const DataMap& map)
{
  Debug::Assert(eq(map, g_Free) || eq(map, g_Scale) || eq(map, g_UniformScale));
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
  if (m_currentMap == g_EulerX)
  {
    if (isMatValid(m_currentMap, mat))
    {
      /// \todo Rotation matrix values validation.
      Debug::Assert(false, "Not implemented yet.");
    }
  }
  else if (m_currentMap == g_Free)
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
  if (m_currentMap == g_Translate)
  {
    if (isMatValid(m_currentMap, mat))
    {
      getData().setValue(mat);
    }
    else
    {
      return EValueSetResult::Err_ConstraintViolation;
    }
  }
  else if (m_currentMap == g_Free)
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
    m_internalData[0].setValue(glm::mat4());
  }
}

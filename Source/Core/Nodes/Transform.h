#pragma once

#include "NodeImpl.h"

namespace Core
{
class Scale : public NodeImpl<ENodeType::Scale>
{
  glm::vec3 m_initialScale;

public:
  explicit Scale(glm::vec3 initialScale = glm::vec3(1.0f), const DataMap& map = g_UniformScale)
      : m_initialScale(initialScale)
  {
    m_initialMap = map;
    m_currentMap = map;
  }
  EValueSetResult setValue(const glm::vec3& vec) override;
  EValueSetResult setValue(const glm::mat4& mat) override;
  void reset() override;
  void setDataMap(const DataMap& map) override;
};

class EulerRotX : public NodeImpl<ENodeType::EulerX>
{
  float m_initialRot;

public:
  explicit EulerRotX(float initialRot = 0.0f, const DataMap& map = g_EulerX)
      : m_initialRot(initialRot)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(float rad) override;
  EValueSetResult setValue(const glm::mat4&) override;
   void reset() override;
};

class EulerRotY : public NodeImpl<ENodeType::EulerY>
{
  float m_initialRot;

public:
  explicit EulerRotY(float initialRot = 0.0f, const DataMap& map = g_EulerY)
      : m_initialRot(initialRot)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(float rad) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
};

class EulerRotZ : public NodeImpl<ENodeType::EulerZ>
{
  float m_initialRot;

public:
  explicit EulerRotZ(float initialRot = 0.0f, const DataMap& map = g_EulerZ)
      : m_initialRot(initialRot)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(float rad) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
};

class Translation : public NodeImpl<ENodeType::Translation>
{
  glm::vec3 m_initialTrans;

public:
  explicit Translation(glm::vec3 initialTrans = glm::vec3(1.0f), const DataMap& map = g_Translate)
      : m_initialTrans(initialTrans)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(const glm::vec3& vec) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
};
}

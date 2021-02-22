#pragma once

#include "NodeImpl.h"

namespace Core
{
class Scale : public NodeBase
{
  glm::vec3 m_initialScale;

public:
  explicit Scale(glm::vec3 initialScale = glm::vec3(1.0f), const DataMap& map = g_UniformScale)
      : NodeBase(getTransformProps(ETransformType::Scale)), m_initialScale(initialScale)
  {
    m_initialMap = map;
    m_currentMap = map;
  }
  EValueSetResult setValue(const glm::vec3& vec) override;
  EValueSetResult setValue(const glm::mat4& mat) override;
  void reset() override;
  void setDataMap(const DataMap& map) override;
  void updateValues(int inputIndex) override;
};

class EulerRotX : public NodeBase
{
  float m_initialRot;

public:
  explicit EulerRotX(float initialRot = 0.0f, const DataMap& map = g_EulerX)
      : NodeBase(getTransformProps(ETransformType::EulerX)), m_initialRot(initialRot)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(float rad) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
  void updateValues(int inputIndex) override;
};

class EulerRotY : public NodeBase
{
  float m_initialRot;

public:
  explicit EulerRotY(float initialRot = 0.0f, const DataMap& map = g_EulerY)
      : NodeBase(getTransformProps(ETransformType::EulerY)), m_initialRot(initialRot)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(float rad) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
  void updateValues(int inputIndex) override;
};

class EulerRotZ : public NodeBase
{
  float m_initialRot;

public:
  explicit EulerRotZ(float initialRot = 0.0f, const DataMap& map = g_EulerZ)
      : NodeBase(getTransformProps(ETransformType::EulerZ)), m_initialRot(initialRot)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(float rad) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
  void updateValues(int inputIndex) override;
};

class Translation : public NodeBase
{
  glm::vec3 m_initialTrans;

public:
  explicit Translation(glm::vec3 initialTrans = glm::vec3(1.0f), const DataMap& map = g_Translate)
      : NodeBase(getTransformProps(ETransformType::Translation)), m_initialTrans(initialTrans)
  {
    m_initialMap = map;
    m_currentMap = map;
  }

  EValueSetResult setValue(const glm::vec3& vec) override;
  EValueSetResult setValue(const glm::mat4&) override;
  void reset() override;
  void updateValues(int inputIndex) override;
};
} // namespace Core

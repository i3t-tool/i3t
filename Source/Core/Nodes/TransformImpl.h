#pragma once

#include "Transform.h"

namespace Core
{
#define I3T_TRANSFORM_CLONE(T)                                                                                         \
  Ptr<Node> clone() override                                                                                           \
  {                                                                                                                    \
    auto node = Builder::createTransform<T>();                                                                         \
                                                                                                                       \
    isLocked() ? node->lock() : node->unlock();                                                                        \
    (hasMenuSynergies() && hasSynergies()) ? node->enableSynergies() : node->disableSynergies();                       \
                                                                                                                       \
    node->setDefaultValues(getDefaultValues());                                                                        \
    node->setValue(getData(0).getMat4());                                                                              \
                                                                                                                       \
    return node;                                                                                                       \
  }
//hasSynergies() ? node->enableSynergies() : node->disableSynergies();                                               \

// PF todo: hasSynergies have only nodes with m_hasMenuSynergies

template <ETransformType T>
class TransformImpl : public Transformation
{};

namespace Builder
{
  template <ETransformType T>
  Ptr<Transformation> createTransform()
  {
    //const auto defaultValues = getTransformDefaults(T);  //\todo PF - not used????
    auto ret = std::make_shared<TransformImpl<T>>();
    ret->init();
    ret->createDefaults();
    ret->initDefaults();

    return ret;
  }
} // namespace Builder


template <>
class TransformImpl<ETransformType::Free> : public Transformation
{
public:
  TransformImpl() : Transformation(getTransformOperation(ETransformType::Free)) {}

  I3T_TRANSFORM_CLONE(ETransformType::Free)

  bool  isValid() const override { return true; }

  [[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override
  {
    setInternalValue(val, coords);
    notifySequence();
    return ValueSetResult{};
  }

  void resetMatrixFromDefaults() override
  {
    //m_isLocked = true; Free is never locked

    setInternalValue(glm::mat4(1.0f));
    notifySequence();
  };
};


template <>
class TransformImpl<ETransformType::Scale> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Scale))
  {
    m_hasMenuSynergies = true;
    enableSynergies();
  }

  I3T_TRANSFORM_CLONE(ETransformType::Scale)

  bool isValid() const override;
	void initDefaults() override;

  [[nodiscard]] ValueSetResult setValue(float val) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
  [[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;
  [[nodiscard]] void           setDefaultUniformScale(float val);

  void resetMatrixFromDefaults() override;
};

/**
 * \code
 *   1      0       0       0
 *   0    cos(T)  -sin(T)   0
 *   0    sin(T)   cos(T)   0
 *   0      0       0       1
 * \endcode
 */
template <>
class TransformImpl<ETransformType::EulerX> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::EulerX))
  {
    m_hasMenuSynergies = true;
    enableSynergies();
  }

  I3T_TRANSFORM_CLONE(ETransformType::EulerX)

  bool isValid() const override;
	void initDefaults() override;

  [[nodiscard]] ValueSetResult setValue(float rad) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
  [[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

  void resetMatrixFromDefaults() override;
};

/**
 * \code
 *  cos(T)   0   sin(T)   0
 *    0      1     0      0
 * -sin(T)   0   cos(T)   0
 *    0      0     0      1
 * \endcode
 */
template <>
class TransformImpl<ETransformType::EulerY> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::EulerY))
  {
    m_hasMenuSynergies = true;
    enableSynergies();
  }

  I3T_TRANSFORM_CLONE(ETransformType::EulerY)

  bool isValid() const override;
	void initDefaults() override;

  [[nodiscard]] ValueSetResult setValue(float rad) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
  [[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

  void resetMatrixFromDefaults() override;
};

/**
 * \code
 *   cos(T)  -sin(T)   0    0
 *   sin(T)   cos(T)   0    0
 *     0        0      1    0
 *     0        0      0    1
 * \endcode
 */
template <>
class TransformImpl<ETransformType::EulerZ> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::EulerZ))
  {
    m_hasMenuSynergies = true;
    enableSynergies();
  }

  I3T_TRANSFORM_CLONE(ETransformType::EulerZ)

  bool isValid() const override;
	void initDefaults() override;

  [[nodiscard]] ValueSetResult setValue(float rad) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
  [[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

  void resetMatrixFromDefaults() override;
};


template <>
class TransformImpl<ETransformType::Translation> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Translation)) {}

  I3T_TRANSFORM_CLONE(ETransformType::Translation)

  bool isValid() const override;
  void initDefaults() override;

  [[nodiscard]] ValueSetResult setValue(float val) override;  // useful for init only, Translation has no synergies
  [[nodiscard]] ValueSetResult setValue(const glm::vec3& vec) override;
  [[nodiscard]] ValueSetResult setValue(const glm::vec4& vec) override;
  [[nodiscard]] ValueSetResult setValue(float val, glm::ivec2 coords) override;

  void resetMatrixFromDefaults() override;
};

//===-- Other transformations ---------------------------------------------===//

template <>
class TransformImpl<ETransformType::AxisAngle> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::AxisAngle)) {}

  I3T_TRANSFORM_CLONE(ETransformType::AxisAngle)

  bool isValid() const override;
	void initDefaults() override;
  
  ValueSetResult setValue(float rads) override;
  ValueSetResult setValue(const glm::vec3& axis) override;

  void resetMatrixFromDefaults() override;
};


/**
 * \brief Quaternion class
 * Quaternion represents the matrix, crated from quaternion.
 * The inner matrix is always created from a normalized quaternion, so it should always represent a rotation.
 * The default value (LOD SetValue) is either a normalized quaternion (when set with synergies enabled),
 * or a not-normalized quaternion (when set with synergies disabled).
 * isValid() checks the normality of the default quaternion (and matrix determinant, which should be 1 all the times).
 */
template <>
class TransformImpl<ETransformType::Quat> : public Transformation
{
  //glm::quat m_initialQuat;  // stored as the defaultValue "quat"
  glm::quat m_normalized;  

public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Quat))
  {
    m_hasMenuSynergies = true;
    enableSynergies(); ///> PF: enableSynergies(); means "normalize" the set quaternion
  }

  I3T_TRANSFORM_CLONE(ETransformType::Quat)

  /**
   * \brief Is the quaternion normalized?
   * \return true if the default "quat" is of unit length
   */
  bool isValid() const override;
  void initDefaults() override;

  /**
   * \brief returns the quaternion or normalized quaternion when synergies
   * \return quaternion
   */
  const glm::quat& getQuat() const;
  const glm::quat& getNormalized() const;

  ValueSetResult setValue(const glm::quat& vec) override;
  ValueSetResult setValue(const glm::vec4& vec) override;

  /**
   * \brief Update matrix to match the default "quat" value. As a side effect, it normalizes the default "quat" for synergies enabled!
   */
  void resetMatrixFromDefaults() override;
};


template <>
class TransformImpl<ETransformType::Ortho> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Ortho))
  {
    m_hasMenuSynergies = true;
    enableSynergies();
  } // PF> enableSynergies(); // means manage symmetric frustum

  I3T_TRANSFORM_CLONE(ETransformType::Ortho)

  bool isValid() const override;
  void initDefaults() override;
  /// No synergies required.
  ValueSetResult setValue(float val, glm::ivec2 coords) override;

  void resetMatrixFromDefaults() override;
};


template <>
class TransformImpl<ETransformType::Perspective> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Perspective)) {}

  I3T_TRANSFORM_CLONE(ETransformType::Perspective)

  bool isValid() const override;
  void initDefaults() override;

  ValueSetResult setValue(float val, glm::ivec2 coords) override;

  void resetMatrixFromDefaults() override;
};


template <>
class TransformImpl<ETransformType::Frustum> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::Frustum))
  {
    m_hasMenuSynergies = true;
    enableSynergies();
  } // PF> enableSynergies(); // means manage symmetric frustum

  I3T_TRANSFORM_CLONE(ETransformType::Frustum)

  bool isValid() const override;
  void initDefaults() override;

  void resetMatrixFromDefaults() override;

  ValueSetResult setValue(float val, glm::ivec2 coords) override;
};


/**
 * Same as perspective projection node, but all values are locked.
 */
template <>
class TransformImpl<ETransformType::LookAt> : public Transformation
{
public:
  explicit TransformImpl() : Transformation(getTransformOperation(ETransformType::LookAt)) {}

  I3T_TRANSFORM_CLONE(ETransformType::LookAt)

  bool isValid() const override;
	void initDefaults() override;
  
  void resetMatrixFromDefaults() override;
  //ValueSetResult setValue(float val, glm::ivec2 coords) override; //PF same as in Transform
};
} // namespace Core

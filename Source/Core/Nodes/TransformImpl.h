/**
 * \file Source/Core/Nodes/TransformImpl.h
 * \author Martin Herich, hericmar@fel.cvut.cz and Petr Felkel, felkepet@fel.cvut.cz (updates)
 *
 * \brief Implementation of individual transformation functionality
 *
 * Transform node
 * ==============
 * Each transform node represents a 4x4 transformation. Each has two types of data, describing them:
 *    1. the 4x4 matrix itself (called the inner value and shown in the Full LOD)
 *    2. the value used in glm to define the transformation (called default values and shown in SetValues LOD)
 *
 * Changing of data in Full LOD are handled by setValue methods specific for each transform.
 * Changing of data in Set Values LOD are handled by a global setDefault("name", value) followed by resetMatrixFromDefaults()
 *
 * Some transformations have direct mapping between matrix and Default values.
 * This mapping is handled in setValues():
 * - the *float* and *vector* parameters set directly the default parameter and create a fresh, new matrix
 * - the version setting a single *value on given coordinates* are more complicated and depend on lock and synergies.
 *   - Setting an editable value in a *locked matrix* does not change the matrix validity and therefore
 *        also sets the default value.
 *   - *Unlocked matrix* setting may invalidate the matrix. Therefore only the editable values can also set the default value,
 *        the other values change only the matrix and invalidate it.
 * - problem remains for *matrix* parameter
 *        Setup passes editable value only - the non-editable parameters remain unchanged!
 *        It uses the setValue of each matrix element, but DOES NOT SET-UP the DEFAULTS
 *        \todo - where is it used?
 *        World/Components/FreeManipulator.cpp::561
 *        State/SerializationVisitor.cpp:445
 *				Source/Core/Nodes/TransformImpl.h/ I3T_TRANSFORM_CLONE macro
 *
 * Test setValue must check:
 *    - float  - setDefault + update matrix - respect synergies 
 *		- vec3   - setDefault + update matrix - respect synergies 
 *		- vec4   - calls vec3 - OK
 *		- mat    - selective copy of values respecting TransformMask
 *		         - not clear, if it clears the rest and respects synergies
 *		- (float, coord) - set single unlocked matrix element
 *		                 - when synergies - may update other synergistic elements
 *		                                  - must update defaults
 *		                 - must not change defaults and the matrix when changing non-synergistic element
 *
 *  Work in progress
 *	----------------
 *
 * | TransformType    | done | mapping Matrix to Default (FULL->SetValues)| Default->Matrix (menu value/reset)                 |
 * | -----------------|------|----------------------------------------|--------------------------------------------------------|
 * |  0 Free          | ok   | no Defaults => no InitDefaults & reset | resetMatrixFromDefaults sets identity   DONE           |
 * |  1 Translate     | ok   | direct   - done in setValue            | own initDefaults(), resetMatrixFromDefaults use default|
 * |  2 EulerX        | ok   | indirect                               |                                                        |
 * |  3 EulerY        | ok   | indirect                               |                                                        |
 * |  4 EulerZ        | ok   | indirect                               |                                                        |
 * |  5 Scale         | ok   | direct                                 |                                                        |
 * |  6 AxisAngle rot | no synergies |                                |                                                        |
 * |  7 Quat          | ok   |                                        |                                                        |
 * |  8 Ortho         | todo | indirect                               |                                                        |
 * |  9 Perspective   | todo | indirect                               |                                                        |
 * | 10 Frustum       | todo | indirect                               |                                                        |
 * | 11 LookAt        | ok   | no mapping                             |  isValid - setValue without test, moved to transform |
 *
 * LookAt 	- isValid checks the linear part of the matrix + unit axes and determinant
 *
 * What should be tested
 * ---------------------
 *	- setValue(float val)              // sets matrix and single float default (such as rotation angle)
 *	- setValue(const glm::vec3& vec)   // sets matrix and single vec3 default (such as rotation axis)
 *	                                   //   or scale vector - than it must follow the synergies
 *	- setValue(const glm::vec4& vec)   // calls setValue(vec3)
 *	- setValue(float val, glm::ivec2 coords);  // setup of a single matrix value - the most complicated
 *	                                         //    must check lock and synergies and set default if possible
 *	- setValue(glm::mat4& m)           // partially copies the given matrix - the editable coords only
 *	                                   //    todo - setValue(mat) does not respect the synergies - It is used in FreeManipulator.cpp,
 *	                                   //    SerializationVisitor, and I3T_TRANSFORM_CLONE macro.
 *	- setDefaultValue - now just sets something, but omits synergies. For quat it is done in resetMatrixFromDefault.
 *	                  Is it the right place?
 */
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

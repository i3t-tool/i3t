/**
 * \file Core/NodeImpl.h
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 18.11.2020
 */
#pragma once

#include "Node.h"

namespace Core
{
/**
 * Implementation of NodeBase class.
 *
 * \tparam T Type of node to be created.
 */
template <ENodeType T> class NodeImpl : public NodeBase
{
public:
  /**
   * Creates an operator of given type (as template parameter).
   *
   * <b>DON'T</b> construct object directly, use Builder::createNode() function.
   *
   * Code taken from I3T v1 NodeImpl<OperatorType>::NodeImpl(...).
   * Operator is no more associated with GUI, as used to be, so no input
   * or output names are assigned here.
   */
  NodeImpl();
  virtual ~NodeImpl() {};

private:
  /**
   * Implementation of virtual function NodeBase::updateValues(int).
   *
   * Implementation differs for each template parameter. See end of the file
   * for more details.
   *
   * \param inputIndex ???
   */
  void updateValues(int inputIndex) override;
};

//===-- Member template function definitions. ------------------------------===//

template <ENodeType T> NodeImpl<T>::NodeImpl() : NodeBase(&operations[T])
{
  // Create input pins.
  for (int i = 0; i < m_operation->numberOfInputs; i++)
  {
    m_inputs.emplace_back(m_operation->inputTypes[i], true, this);
  }

  // Create output pins and data storage for each output.
  for (int i = 0; i < m_operation->numberOfOutputs; i++)
  {
    m_outputs.emplace_back(m_operation->outputTypes[i], false, this);
    m_internalData.emplace_back();
  }

  /// \todo Do not call virtual member function from constructor
  /// https://isocpp.org/wiki/faq/strange-inheritance#calling-virtuals-from-ctors
  updateValues(0);
}

//===-----------------------------------------------------------------------===//
// build error: there are no arguments to ‘I3T_DEBUG_ASSERT’ that depend on a template parameter, so a declaration of ‘I3T_DEBUG_ASSERT’ must be available [-fpermissive]
//template <ENodeType T> void NodeImpl<T>::updateValues(int inputIndex)
//{
//  I3T_DEBUG_ASSERT(false, "Calling update function of unimplemented NodeImpl type.");
//}

// inversion
template <> FORCE_INLINE void NodeImpl<ENodeType::Inversion>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::inverse(m_inputs[0].getStorage().getMat4()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// tranpose
template <> FORCE_INLINE void NodeImpl<ENodeType::Transpose>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::transpose(m_inputs[0].getStorage().getMat4()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// determinant
template <> FORCE_INLINE void NodeImpl<ENodeType::Determinant>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::determinant(m_inputs[0].getStorage().getMat4()));
  }
  else
  {
    m_internalData[0].setValue(1.0f);
  }
}

// mat * mat
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixMulMatrix>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    auto mat1 = m_inputs[0].getStorage().getMat4();
    auto mat2 = m_inputs[1].getStorage().getMat4();
    m_internalData[0].setValue(mat1 * mat2);
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getMat4());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getMat4());
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// MatrixAddMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixAddMatrix>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getMat4() + m_inputs[1].getStorage().getMat4());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getMat4());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getMat4());
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// MatrixMulVector
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixMulVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getMat4() * m_inputs[1].getStorage().getVec4());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec4());
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// VectorMulMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorMulMatrix>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    glm::vec4 vec = m_inputs[0].getStorage().getVec4();
    glm::mat4 mat = m_inputs[1].getStorage().getMat4();
    m_internalData[0].setValue(vec * mat);
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4());
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// FloatMulMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixMulFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    glm::mat4 mat = m_inputs[0].getStorage().getMat4();
    float num = m_inputs[1].getStorage().getFloat();
    m_internalData[0].setValue(num * mat);
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat() * glm::mat4());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getMat4());
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

template <> FORCE_INLINE void NodeImpl<ENodeType::VectorDotVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    auto vec1 = m_inputs[0].getStorage().getVec4();
    auto vec2 = m_inputs[1].getStorage().getVec4();
    m_internalData[0].setValue(glm::dot(vec1, vec2));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// VectorAddVector
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorAddVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4() + m_inputs[1].getStorage().getVec4());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec4());
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// VectorSubVector
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorSubVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4() - m_inputs[1].getStorage().getVec4());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec4());
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// VectorMulFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorMulFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec4() * m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec4());
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// VectorPerspectiveDivision
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorPerspectiveDivision>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    glm::vec4 in = m_inputs[0].getStorage().getVec4();
    m_internalData[0].setValue(glm::vec4(in.x / in.w, in.y / in.w, in.z / in.w, 1.0f));
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// NormalizeVector
template <> FORCE_INLINE void NodeImpl<ENodeType::NormalizeVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::normalize(m_inputs[0].getStorage().getVec4()));
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// MixVector
template <> FORCE_INLINE void NodeImpl<ENodeType::MixVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(glm::mix(m_inputs[0].getStorage().getVec4(), m_inputs[1].getStorage().getVec4(),
                                        m_inputs[2].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec4());
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// Vector3CrossVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3CrossVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::cross(m_inputs[0].getStorage().getVec3(), m_inputs[1].getStorage().getVec3()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec3());
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3DotVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    auto vec1 = m_inputs[0].getStorage().getVec3();
    auto vec2 = m_inputs[1].getStorage().getVec3();
    m_internalData[0].setValue(glm::dot(vec1, vec2));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// Vector3AddVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3AddVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3() + m_inputs[1].getStorage().getVec3());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec3());
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

// Vector3SubVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3SubVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3() - m_inputs[1].getStorage().getVec3());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec3());
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

// Vector3MulFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3MulFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec3() * m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec3());
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

// NormalizeVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::NormalizeVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::normalize(m_inputs[0].getStorage().getVec3()));
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

// Vector3Length
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3Length>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::length(m_inputs[0].getStorage().getVec3()));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// ShowVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::ShowVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {

    // m_internalData[0].setValue(glm::normalize(m_inputs[0].getStorage().getVec3()));

    glm::mat4 m = glm::mat4();

    glm::vec3 inNor = glm::normalize(m_inputs[0].getStorage().getVec3());

    float angle = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), inNor));

    if (angle != 0)
    {
      glm::vec3 cross = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), inNor);

      if (glm::length2(cross) != 0)
        m = glm::rotate(m, angle, glm::normalize(cross));
      else
        m = glm::rotate(m, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    m[0] = glm::vec4(m_inputs[0].getStorage().getVec3(), 0.0f);

    m_internalData[0].setValue(m);
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// MixVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::MixVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(glm::mix(m_inputs[0].getStorage().getVec3(), m_inputs[1].getStorage().getVec3(),
                                        m_inputs[2].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getVec3());
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

// ConjQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::ConjQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::conjugate(m_inputs[0].getStorage().getQuat()));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// FloatVecToQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatVecToQuat>::updateValues(int inputIndex)
{
  glm::quat tmp = glm::quat(); // PF (1,0,0,0) if (nothing connected) w=1; else w=0;

  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::quat(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getVec3()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::quat(m_inputs[0].getStorage().getFloat(), glm::vec3()));
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::quat(0.0f, m_inputs[1].getStorage().getVec3()));
  }
  else
    m_internalData[0].setValue(glm::quat());
}

// AngleAxisToQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::AngleAxisToQuat>::updateValues(int inputIndex)
{
  /*
  \todo
  if (m_inputs[0].isPluggedIn()) {
    m_inputs[0].setVisible(true);
    m_inputs[1]->setVisible(false);
  }
  else
  if (m_inputs[1].isPluggedIn()) {
    m_inputs[1]->setVisible(true);
    m_inputs[0]->setVisible(false);
  }
  else {
    m_inputs[0]->setVisible(true);
    m_inputs[1]->setVisible(true);
  }
   */

  // angle - is divided by 2 in angleAxes
  if (m_inputs[0].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(
        glm::angleAxis(m_inputs[0].getStorage().getFloat(), glm::normalize(m_inputs[2].getStorage().getVec3())));
  }
  else if (m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    // angle / 2  - given a halfAngle - multiply by 2, as needed by angleAxis

    m_internalData[0].setValue(glm::angleAxis(m_inputs[1].getStorage().getFloat() * 2.0f,
                                              glm::normalize(m_inputs[2].getStorage().getVec3())));
  }
  else if (m_inputs[2].isPluggedIn())
  {
    // m_internalData[0].setValue(glm::quat(0.0f, m_inputs[2].getStorage().getVec3()));
    m_internalData[0].setValue(glm::angleAxis(
        0.0f, glm::normalize(m_inputs[2].getStorage().getVec3()))); // bud zadat 1.0f, nebo angleAxis(0.0f,...)
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// VecVecToQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::VecVecToQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::quat(glm::normalize(m_inputs[0].getStorage().getVec3()),
                                         glm::normalize(m_inputs[1].getStorage().getVec3())));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatToFloatVec
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatToFloatVec>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getQuat()[3]); // w
    m_internalData[1].setValue(m_inputs[0].getStorage().getVec3());    //(x,y,z)
  }
  else
  {
    m_internalData[0].setValue(1.0f); // PF - to return a unit quaternion
    m_internalData[1].setValue(glm::vec3(0.0f, 0.0f, 0.0f));
  }
}

// QuatToAngleAxis
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatToAngleAxis>::updateValues(int inputIndex)
{
  // angle
  if (m_inputs[0].isPluggedIn())
  {

    // if (SetupForm::radians)	m_internalData[0].setValue(glm::angle(m_inputs[0].getStorage().getQuat()));
    // else m_internalData[0].setValue(glm::degrees(glm::angle(m_inputs[0].getStorage().getQuat())));

    m_internalData[0].setValue(glm::angle(m_inputs[0].getStorage().getQuat()));

    m_internalData[1].setValue(glm::axis(m_inputs[0].getStorage().getQuat()));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
    m_internalData[1].setValue(glm::vec3()); // (0,0,1)
  }
}

// QuatToEuler
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatToEuler>::updateValues(int inputIndex)
{
  // angle
  if (m_inputs[0].isPluggedIn())
  {

    glm::vec3 e = glm::eulerAngles(m_inputs[0].getStorage().getQuat());

    m_internalData[0].setValue(e.x);
    m_internalData[1].setValue(e.y);
    m_internalData[2].setValue(e.z);
  }
  else
  {
    m_internalData[0].setValue(0.0f);
    m_internalData[1].setValue(0.0f);
    m_internalData[2].setValue(0.0f);
  }
}

// EulerToQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::EulerToQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {

    m_internalData[0].setValue(
        glm::quat(glm::vec3(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
                            m_inputs[2].getStorage().getFloat())));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatInverse
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatInverse>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::inverse(m_inputs[0].getStorage().getQuat()));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatSlerp
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatSlerp>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    // m_internalData[0].setValue(glm::mix(m_inputs[0].getStorage().getQuat(), m_inputs[1].getStorage().getQuat(),
    // m_inputs[2].getStorage().getFloat()));
    m_internalData[0].setValue(glm::slerp(m_inputs[0].getStorage().getQuat(), m_inputs[1].getStorage().getQuat(),
                                          m_inputs[2].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

/// PF: Long way version of Spherical linear interpolation of two quaternions.
/// The interpolation always take the long path and the rotation is performed at constant speed.
///
/// @param x A quaternion
/// @param y A quaternion
/// @param a Interpolation factor. The interpolation is defined beyond the range [0, 1].
/// @tparam T Value type used to build the quaternion. Supported: half, float or double.
/// @see gtc_quaternion
template <typename T, glm::precision P>
GLM_FUNC_DECL glm::tquat<T, P> longWaySlerp(glm::tquat<T, P> const& x, glm::tquat<T, P> const& y, T a);

template <typename T, glm::precision P>
GLM_FUNC_QUALIFIER glm::tquat<T, P> longWaySlerp(glm::tquat<T, P> const& x, glm::tquat<T, P> const& y, T a)
{
  glm::tquat<T, P> z = y;

  T cosTheta = dot(x, y);

  // If cosTheta < 0, the interpolation will take the long way around the sphere.
  // To fix this, one quat must be negated.
  // PF force the long way round interpolation
  if (cosTheta > T(0))
  {
    z = -y;
    cosTheta = -cosTheta;
  }

  // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero
  // denominator
  if (cosTheta > T(1) - glm::epsilon<T>())
  {
    // Linear interpolation
    return glm::tquat<T, P>(glm::mix(x.w, z.w, a), glm::mix(x.x, z.x, a), glm::mix(x.y, z.y, a),
                            glm::mix(x.z, z.z, a));
  }
  else
  {
    // Essential Mathematics, page 467
    T angle = acos(cosTheta);
    /// \todo operator* override (float and double?)
    // return (sin((T(1) - a) * angle) * x + sin(a * angle) * z) / sin(angle);
    return glm::tquat<T, P>(
        glm::mix(x.w, z.w, a), glm::mix(x.x, z.x, a), glm::mix(x.y, z.y, a),
        glm::mix(x.z, z.z,
                 a)); // tohle je určitě špatně - je to tu jen abych to zkompiloval - má tu být to o řádek výš
  }
}

// QuatLongWaySlerp
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatLongWaySlerp>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    // m_internalData[0].setValue(glm::mix(m_inputs[0].getStorage().getQuat(), m_inputs[1].getStorage().getQuat(),
    // m_inputs[2].getStorage().getFloat()));
    m_internalData[0].setValue(longWaySlerp(m_inputs[0].getStorage().getQuat(), m_inputs[1].getStorage().getQuat(),
                                            m_inputs[2].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatLerp
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatLerp>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {

    float w = glm::mix(m_inputs[0].getStorage().getQuat().w, m_inputs[1].getStorage().getQuat().w,
                       m_inputs[2].getStorage().getFloat());
    float x = glm::mix(m_inputs[0].getStorage().getQuat().x, m_inputs[1].getStorage().getQuat().x,
                       m_inputs[2].getStorage().getFloat());
    float y = glm::mix(m_inputs[0].getStorage().getQuat().y, m_inputs[1].getStorage().getQuat().y,
                       m_inputs[2].getStorage().getFloat());
    float z = glm::mix(m_inputs[0].getStorage().getQuat().z, m_inputs[1].getStorage().getQuat().z,
                       m_inputs[2].getStorage().getFloat());

    m_internalData[0].setValue(glm::quat(w, x, y, z));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// FloatMulQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatMulQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat() * m_inputs[1].getStorage().getQuat());
  }
  else if (m_inputs[0].isPluggedIn())
  { // predpoklada quat (1,0,0,0)
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat() * glm::quat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getQuat()); // PF new predpoklada float == 1
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatMulQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatMulQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getQuat() * m_inputs[1].getStorage().getQuat());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getQuat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getQuat());
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatVecConjQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatVecConjQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    // chybna varianta - - nasobi qv jako (quat * vec3) * conj(q), tj. 2x za sebou vec3 * quat -> otoci o 2x uhel
    // m_internalData[0].setValue(m_inputs[0].getStorage().getQuat() * m_inputs[1].getStorage().getVec3() *
    // glm::conjugate(m_inputs[0].getStorage().getQuat())); // chyba

    // spravna varianta, vycislujici vzorec qvq* jako q * quat(v) * q*
    m_internalData[0].setValue(m_inputs[0].getStorage().getQuat() *
                               glm::quat(0.0f, m_inputs[1].getStorage().getVec3()) *
                               glm::conjugate(m_inputs[0].getStorage().getQuat()));

    // spravna varianta vyuzivajici glm::rotate
    // m_internalData[0].setValue(m_inputs[0].getStorage().getQuat() * m_inputs[1].getStorage().getVec3() );   // 1x
    // q * v
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getQuat());
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatLength
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatLength>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::length(m_inputs[0].getStorage().getQuat()));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// ClampFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::ClampFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(glm::clamp(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
                                          m_inputs[2].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(
        glm::clamp(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(), 1.0f));
  }
  else if (m_inputs[0].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(
        glm::clamp(m_inputs[0].getStorage().getFloat(), 0.0f, m_inputs[2].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::clamp(m_inputs[0].getStorage().getFloat(), 0.0f, 1.0f));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// FloatMulFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatMulFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat() * m_inputs[1].getStorage().getFloat());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getFloat());
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// FloatDivFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatDivFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat() / m_inputs[1].getStorage().getFloat());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(1.0f / m_inputs[1].getStorage().getFloat());
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}
// FloatAddFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatAddFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat() + m_inputs[1].getStorage().getFloat());
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getFloat());
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// FloatPowFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatPowFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(pow(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getFloat());
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// MixFloat
template <> FORCE_INLINE void NodeImpl<ENodeType::MixFloat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(glm::mix(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
                                        m_inputs[2].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat());
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[1].getStorage().getFloat());
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// FloatSinCos
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatSinCos>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(sin(m_inputs[0].getStorage().getFloat()));
    m_internalData[1].setValue(cos(m_inputs[0].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
    m_internalData[0].setValue(1.0f);
  }
}

// ASinACos
template <> FORCE_INLINE void NodeImpl<ENodeType::ASinACos>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(asin(m_inputs[0].getStorage().getFloat()));
    m_internalData[1].setValue(acos(m_inputs[0].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
    m_internalData[0].setValue(1.0f);
  }
}

// Signum
template <> FORCE_INLINE void NodeImpl<ENodeType::Signum>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    float inputValue = m_inputs[0].getStorage().getFloat();
    m_internalData[0].setValue((inputValue > 0) ? 1.0f : ((inputValue < 0) ? -1.0f : 0.0f));
  }
  else
  {
    m_internalData[0].setValue(0.0f);
  }
}

// MatrixToVectors
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixToVectors>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getMat4()[0]);
    m_internalData[1].setValue(m_inputs[0].getStorage().getMat4()[1]);
    m_internalData[2].setValue(m_inputs[0].getStorage().getMat4()[2]);
    m_internalData[3].setValue(m_inputs[0].getStorage().getMat4()[3]);
  }
  else
  {
    m_internalData[0].setValue(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
    m_internalData[1].setValue(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    m_internalData[2].setValue(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    m_internalData[3].setValue(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
  }
}

// Vectors3ToMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::Vectors3ToMatrix>::updateValues(int inputIndex)
{
  glm::mat4 tmp = glm::mat4();

  if (m_inputs[0].isPluggedIn())
    tmp[0] = glm::vec4(m_inputs[0].getStorage().getVec3(), 0.0f);
  if (m_inputs[1].isPluggedIn())
    tmp[1] = glm::vec4(m_inputs[1].getStorage().getVec3(), 0.0f);
  if (m_inputs[2].isPluggedIn())
    tmp[2] = glm::vec4(m_inputs[2].getStorage().getVec3(), 0.0f);
  if (m_inputs[3].isPluggedIn())
    tmp[3] = glm::vec4(m_inputs[3].getStorage().getVec3(), 1.0f);

  m_internalData[0].setValue(tmp);
}

// VectorsToMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorsToMatrix>::updateValues(int inputIndex)
{
  glm::mat4 tmp = glm::mat4();

  if (m_inputs[0].isPluggedIn())
    tmp[0] = m_inputs[0].getStorage().getVec4();
  if (m_inputs[1].isPluggedIn())
    tmp[1] = m_inputs[1].getStorage().getVec4();
  if (m_inputs[2].isPluggedIn())
    tmp[2] = m_inputs[2].getStorage().getVec4();
  if (m_inputs[3].isPluggedIn())
    tmp[3] = m_inputs[3].getStorage().getVec4();

  m_internalData[0].setValue(tmp);
}

// MatrixToFloats
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixToFloats>::updateValues(int inputIndex)
{
  glm::mat4 tmp = glm::mat4();

  if (m_inputs[0].isPluggedIn())
  {
    tmp = m_inputs[0].getStorage().getMat4();
  }

  for (int i = 0; i < 4; i++)
  {
    for (int k = 0; k < 4; k++)
    {
      m_internalData[i * 4 + k].setValue(tmp[i][k]);
    }
  }
}

// FloatsToMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatsToMatrix>::updateValues(int inputIndex)
{
  glm::mat4 tmp = glm::mat4();

  for (int i = 0; i < 4; i++)
  {
    for (int k = 0; k < 4; k++)
    {
      if (m_inputs[i * 4 + k].isPluggedIn())
        tmp[i][k] = m_inputs[i * 4 + k].getStorage().getFloat();
    }
  }

  m_internalData[0].setValue(tmp);
}

// MatrixToTR
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixToTR>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[1].setValue(glm::mat4(glm::mat3(m_inputs[0].getStorage().getMat4())));
    glm::mat4 T = glm::mat4();
    T[3] = glm::vec4(glm::vec3(m_inputs[0].getStorage().getMat4()[3]), 1.0f);
    m_internalData[0].setValue(T);
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
    m_internalData[1].setValue(glm::mat4());
  }
}

// TRToMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::TRToMatrix>::updateValues(int inputIndex)
{
  glm::mat4 tmp = glm::mat4();

  if (m_inputs[0].isPluggedIn())
  {
    tmp[3] = glm::vec4(glm::vec3(m_inputs[0].getStorage().getMat4()[3]), 1.0f);
  }

  if (m_inputs[1].isPluggedIn())
  {
    tmp[0] = glm::vec4(glm::vec3(m_inputs[1].getStorage().getMat4()[0]), 0.0f);
    tmp[1] = glm::vec4(glm::vec3(m_inputs[1].getStorage().getMat4()[1]), 0.0f);
    tmp[2] = glm::vec4(glm::vec3(m_inputs[1].getStorage().getMat4()[2]), 0.0f);
  }

  m_internalData[0].setValue(tmp);
}

// MatrixToQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::MatrixToQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::quat_cast(m_inputs[0].getStorage().getMat4()));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// QuatToMatrix
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatToMatrix>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::mat4_cast(m_inputs[0].getStorage().getQuat()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// VectorToFloats
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorToFloats>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec4()[0]);
    m_internalData[1].setValue(m_inputs[0].getStorage().getVec4()[1]);
    m_internalData[2].setValue(m_inputs[0].getStorage().getVec4()[2]);
    m_internalData[3].setValue(m_inputs[0].getStorage().getVec4()[3]);
  }
  else
  {
    m_internalData[0].setValue(0.0f);
    m_internalData[1].setValue(0.0f);
    m_internalData[2].setValue(0.0f);
    m_internalData[3].setValue(0.0f);
  }
}

// FloatsToVector
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatsToVector>::updateValues(int inputIndex)
{
  glm::vec4 tmp = glm::vec4();

  if (m_inputs[0].isPluggedIn())
    tmp[0] = m_inputs[0].getStorage().getFloat();
  if (m_inputs[1].isPluggedIn())
    tmp[1] = m_inputs[1].getStorage().getFloat();
  if (m_inputs[2].isPluggedIn())
    tmp[2] = m_inputs[2].getStorage().getFloat();
  if (m_inputs[3].isPluggedIn())
    tmp[3] = m_inputs[3].getStorage().getFloat();

  m_internalData[0].setValue(tmp);
}

// VectorTVector3ToFloatsoFloats
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3ToFloats>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3()[0]);
    m_internalData[1].setValue(m_inputs[0].getStorage().getVec3()[1]);
    m_internalData[2].setValue(m_inputs[0].getStorage().getVec3()[2]);
  }
  else
  {
    m_internalData[0].setValue(0.0f);
    m_internalData[1].setValue(0.0f);
    m_internalData[2].setValue(0.0f);
  }
}

// FloatsToVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatsToVector3>::updateValues(int inputIndex)
{
  glm::vec3 tmp = glm::vec3();

  if (m_inputs[0].isPluggedIn())
    tmp[0] = m_inputs[0].getStorage().getFloat();
  if (m_inputs[1].isPluggedIn())
    tmp[1] = m_inputs[1].getStorage().getFloat();
  if (m_inputs[2].isPluggedIn())
    tmp[2] = m_inputs[2].getStorage().getFloat();

  m_internalData[0].setValue(tmp);
}

// VectorToVector3
template <> FORCE_INLINE void NodeImpl<ENodeType::VectorToVector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::vec3(m_inputs[0].getStorage().getVec4()));
  }
  else
  {
    m_internalData[0].setValue(glm::vec3());
  }
}

// Vector3ToVector
template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3ToVector>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::vec4(m_inputs[0].getStorage().getVec3(), m_inputs[1].getStorage().getFloat()));
  }
  else if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::vec4(m_inputs[0].getStorage().getVec3(), 0.0f)); // PF implicitne pro vektor se
                                                                                     // w=0
  }
  else if (m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::vec4(
        glm::vec3(), m_inputs[1].getStorage().getFloat())); // PF implicitne pro nezapojeny vektor kopiruje w=0
  }
  else
  {
    m_internalData[0].setValue(glm::vec4());
  }
}

// QuatToFloats
template <> FORCE_INLINE void NodeImpl<ENodeType::QuatToFloats>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getQuat()[0]);
    m_internalData[1].setValue(m_inputs[0].getStorage().getQuat()[1]);
    m_internalData[2].setValue(m_inputs[0].getStorage().getQuat()[2]);
    m_internalData[3].setValue(m_inputs[0].getStorage().getQuat()[3]);
  }
  else
  {
    m_internalData[0].setValue(1.0f); // PF - to return a unit quaternion
    m_internalData[1].setValue(0.0f);
    m_internalData[2].setValue(0.0f);
    m_internalData[3].setValue(0.0f);
  }
}

// FloatsToQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::FloatsToQuat>::updateValues(int inputIndex)
{
  glm::quat tmp = glm::quat(); // PF (1,0,0,0) if (nothing connected) w=1; else w=0;

  if (m_inputs[0].isPluggedIn() || m_inputs[1].isPluggedIn() || m_inputs[2].isPluggedIn())
    tmp[3] = 0.0f;

  if (m_inputs[0].isPluggedIn())
    tmp[0] = m_inputs[0].getStorage().getFloat();
  if (m_inputs[1].isPluggedIn())
    tmp[1] = m_inputs[1].getStorage().getFloat();
  if (m_inputs[2].isPluggedIn())
    tmp[2] = m_inputs[2].getStorage().getFloat();
  if (m_inputs[3].isPluggedIn())
    tmp[3] = m_inputs[3].getStorage().getFloat(); // w

  m_internalData[0].setValue(tmp);
}

// NormalizeQuat
template <> FORCE_INLINE void NodeImpl<ENodeType::NormalizeQuat>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(glm::normalize(m_inputs[0].getStorage().getQuat()));
  }
  else
  {
    m_internalData[0].setValue(glm::quat());
  }
}

// Translation
template <> FORCE_INLINE void NodeImpl<ENodeType::Translation>::updateValues(int inputIndex)
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

// EulerX
template <> FORCE_INLINE void NodeImpl<ENodeType::EulerX>::updateValues(int inputIndex)
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

// EulerY
template <> FORCE_INLINE void NodeImpl<ENodeType::EulerY>::updateValues(int inputIndex)
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

// EulerZ
template <> FORCE_INLINE void NodeImpl<ENodeType::EulerZ>::updateValues(int inputIndex)
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

// AxisAngle
template <> FORCE_INLINE void NodeImpl<ENodeType::AxisAngle>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn())
  {
    m_internalData[0].setValue(glm::rotate(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getVec3()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// Scale
template <> FORCE_INLINE void NodeImpl<ENodeType::Scale>::updateValues(int inputIndex)
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

// Ortho
template <> FORCE_INLINE void NodeImpl<ENodeType::Ortho>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn() &&
      m_inputs[3].isPluggedIn() && m_inputs[4].isPluggedIn() && m_inputs[5].isPluggedIn())
  {
    m_internalData[0].setValue(glm::ortho(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
                                          m_inputs[2].getStorage().getFloat(), m_inputs[3].getStorage().getFloat(),
                                          m_inputs[4].getStorage().getFloat(), m_inputs[5].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// perspective
template <> FORCE_INLINE void NodeImpl<ENodeType::Perspective>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn() &&
      m_inputs[3].isPluggedIn())
  {
    m_internalData[0].setValue(
        glm::perspective(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
                         m_inputs[2].getStorage().getFloat(), m_inputs[3].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// Frustum
template <> FORCE_INLINE void NodeImpl<ENodeType::Frustum>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn() &&
      m_inputs[3].isPluggedIn() && m_inputs[4].isPluggedIn() && m_inputs[5].isPluggedIn())
  {
    m_internalData[0].setValue(glm::frustum(m_inputs[0].getStorage().getFloat(), m_inputs[1].getStorage().getFloat(),
                                            m_inputs[2].getStorage().getFloat(), m_inputs[3].getStorage().getFloat(),
                                            m_inputs[4].getStorage().getFloat(),
                                            m_inputs[5].getStorage().getFloat()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

// LookAt
template <> FORCE_INLINE void NodeImpl<ENodeType::LookAt>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
  {
    m_internalData[0].setValue(glm::lookAt(m_inputs[0].getStorage().getVec3(), m_inputs[1].getStorage().getVec3(),
                                           m_inputs[2].getStorage().getVec3()));
  }
  else
  {
    m_internalData[0].setValue(glm::mat4());
  }
}

template <> FORCE_INLINE void NodeImpl<ENodeType::Vector3>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getVec3());
  }
}

template <> FORCE_INLINE void NodeImpl<ENodeType::Float>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getFloat());
  }
}

template <> FORCE_INLINE void NodeImpl<ENodeType::Matrix>::updateValues(int inputIndex)
{
  if (m_inputs[0].isPluggedIn())
  {
    m_internalData[0].setValue(m_inputs[0].getStorage().getMat4());
  }
}
} // namespace Core

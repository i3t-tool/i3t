# How to add node to I3T

## pin and wire types
- Operations.h: defaultIoNames - strings pulse, float, vec3, vec4, matrix, and quad (no string for Matrix_MULL and SCREEN)
- NodeData.h  : EValueType     - Pulse, Float, Vec3, Vec4, Matrix, Quat, MatrixMul, Screen, Ptr,
Data is stored in the DatastoreClass in an std::variant structure, data type is in ''opValueType''. 
                 Access is via getters ''getFloat'', ... ,and a ''setValue'' template.


## Node types
There are three node types
- operator (blue box, serves as a calculator of the output from the connected inputs having no inner state )
  They hav a subgroup called Constructors
- sequence (envelope for matrices)
- transformation (yellow box, stores the 4x4 matrix of the particular transformation)

## How to add the transformation node
### Adding a new box to ``Operations.h``:

a) Operators:
- create a new enum in ``enum class EOperatorType``
	- simple nodes that just compute output from given inputs
	- constructors create a matrix of given type (such as translation or EulerX rotation)

b) Matrices (Transformations):
- create a new enum in ``enum class ETransformType``	

- Prepare list of input and output pins into ``static const std::vector<EValueType>`` if it is not already there.
- Prepare list of pin names into ``static const std::vector<std::string>s``

- Add the new node into the vector of operations ()
  - Operator: ``{n(EOperatorType::Inversion), "inversion", 1, matrixInput, 1, matrixInput},``
	- Transformation: ``{n(EOperatorType::MakeTranslation), "translate", 1, vector3Input, 1, matrixInput}, // translate``
	- Use one of predefined version of ''Operation'' constructor, depending on existence of inputs, outputs, hover tag and explicit pin labels

- Transform matrices have one more configuration vector ''ETransformType'' 
  - predefined pair of type and name (ETransformType and string)
	- 16 bit info about editable values of a locked matrix
	- vector of editable parameters in the middle LOD (SetValues) - pairs param_name data_type EValueType)
	
### Writing the operator logic in Core
Prepare the function ``updateValues()`` of the operator (specialization of the ``Operator`` template). 

An example for conversion from Euler angles to Quaternion:
```// EulerToQuat
template <> FORCE_INLINE void Operator<EOperatorType::EulerToQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{

		setInternalValue(
				glm::quat(glm::vec3(m_inputs[0].data().getFloat(), m_inputs[1].data().getFloat(),
														m_inputs[2].data().getFloat())));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}
```	
	
### Writing the 
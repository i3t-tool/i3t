# How to add node to I3T

This document describes how to add a new node to the I3T application.
It is not so complex, but requires changes in several places in the code.

## Pin and wire types

- `Core/Operations.h`: `defaultIoNames` - strings pulse, float, vec3, vec4, matrix, and quad (no string for Matrix_MULL
  and SCREEN)
- `Core/NodeData.h`: `EValueType`       - Pulse, Float, Vec3, Vec4, Matrix, Quat, MatrixMul, Screen, Ptr,
  Data is stored in the DatastoreClass in a `std::variant` structure, data type is in `opValueType`.
  Access is via getters `getFloat`, ..., and a `setValue` template.

## Node types

There are three node types:

- operator (blue box, serves as a calculator of the output from the connected inputs having no inner state)
  They have a subgroup called Constructors
- sequence (envelope for matrices)
- transformation (yellow box, stores the 4x4 matrix of the particular transformation)

## Step-by-step guide

### Adding a new box to `Operations.h`:

1. Operators:
    - create a new enum in ``enum class EOperatorType``
        - simple nodes that just compute output from given inputs
        - constructors create a matrix of given type (such as translation or EulerX rotation)

2. Matrices (Transformations):
    - create a new enum in ``enum class ETransformType``

    - Prepare list of input and output pins into ``static const std::vector<EValueType>`` if it is not already there.
    - Prepare list of pin names into ``static const std::vector<std::string>``

    - Add the new node into the vector of operations ()
        - Operator: ``{n(EOperatorType::Inversion), "inversion", 1, matrixInput, 1, matrixInput},``
            - Transformation:
              ``{n(EOperatorType::MakeTranslation), "translate", 1, vector3Input, 1, matrixInput}, // translate``
            - Use one of predefined version of ''Operation'' constructor, depending on existence of inputs, outputs,
              hover tag and explicit pin labels

    - Transform matrices have one more configuration vector ''ETransformType''
        - predefined pair of type and name (ETransformType and string)
            - 16 bit info about editable values of a locked matrix
            - vector of editable parameters in the middle LOD (SetValues) - pairs param_name data_type EValueType)

### Writing the operator logic in Core

Prepare the function ``updateValues()`` of the operator (specialization of the ``Operator`` template).

An example for conversion from Euler angles to Quaternion:

```cpp
// EulerToQuat
template <> FORCE_INLINE void Operator<EOperatorType::EulerToQuat>::updateValues(int inputIndex)
{
	if (m_inputs[0].isPluggedIn() && m_inputs[1].isPluggedIn() && m_inputs[2].isPluggedIn())
	{
		glm::vec3 value(
			m_inputs[0].data().getFloat(), 
			m_inputs[1].data().getFloat(),
			m_inputs[2].data().getFloat()
		); 
		setInternalValue(glm::quat(value));
	}
	else
	{
		setInternalValue(glm::quat());
	}
}
```

### Writing the transformation logic in Core

- Create a new class template specialization derived from the `TransformationImpl<ETransformType>` class template.

- Implement the `updateValues()`, `updateDefaultValues()`, and `reset()` functions.

### Writing graphical representation in GUI

1. Operators and Matrices (Transformation) - You don't need to do anything, the GUI will automatically create the node
   for you.

2. For custom node you will need to create class in `GUI/Elements/Nodes/`:
    - Create a new class derived from `DIWNE::CoreNodeWithPins`.
    - Implement the constructor and the `centerContent()` method. Optionally also `leftContent()` and `rightContent()`
      methods to add custom pins, otherwise pins will be created automatically based on the underlying Core node.
    - Implement the `onDestroy()` function if needed.

Do not forget to add the new node to the "Add..." popup menu in `WorkspaceDiwne.h`.

### Writing serialization and deserialization

If you created Operator or Transformation, you don't need to do anything.
The serialization and deserialization is done automatically.
But if you created a custom node, you need to implement custom serialization and deserialization.

Serialization is done through the [`NodeVisitor`](../cpp-api-reference/classes/classNodeVisitor.html) class.
You have to create new `visit` function for your node type there, and supply the serialization logic
in derived classes - [`SerializationVisitor`](../cpp-api-reference/classes/classSerializationVisitor.html)
to serialize the node to JSON, and `ToScriptVisitor` to serialize the node to a script.

Deserialization is needed for JSON only, see the `State/NodeDeserializer.cpp` code for more details.

/**
 * File with operations table.
 */
#pragma once

#include <bitset>
#include <map>
#include <optional>
#include <sstream>

#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/transform.hpp"

#include "Core/Defs.h"

#include "NodeData.h"

namespace Core
{
static const std::vector<std::string> emptyNames = {};

#define NO_TAG ""
#define DEFAULT_NAMES emptyNames

static const std::vector<std::string> matrixIndexNames()
{
	std::vector<std::string> names;
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			std::stringstream ss;
			ss << i << ", " << k;
			names.push_back(ss.str());
		}
	}
	return names;
}

using PinGroup = std::vector<EValueType>;

/**
 * \brief Description of each graph node (operation enum, default label, input
 * and output pin names and types)
 */
struct Operation
{
	using Pins = std::vector<EValueType>;

	const std::string keyWord;
	const std::string defaultLabel;
	const int numberOfInputs; ///< \todo MH Remove, size is known at compile time.
	const std::vector<EValueType> inputTypes;
	const int numberOfOutputs; ///< \todo MH Remove, size is known at compile time.
	const std::vector<EValueType> outputTypes;
	const std::string defaultTagText = NO_TAG;
	const std::vector<std::string> defaultInputNames =
	    DEFAULT_NAMES; // if the names differ from the names of the OpValueType
	const std::vector<std::string> defaultOutputNames =
	    DEFAULT_NAMES; // if the names differ from the names of the OpValueType

	const bool isConstructor = false; //< this is a standard operator node - processes only the connected
	                                  // pins and has no GUI  editable values

	/**
	 * \brief Constructor used for definition of editable values in static const
	 * std::vector<TransformOperation> g_transforms \param keyWord enum defined in
	 * enum class EOperatorType \param defaultLabel default string label in the
	 * workspace \todo Q which one is used - this or the label from
	 */
	Operation(const std::string& keyWord, const std::string& defaultLabel)
	    : keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(0), numberOfOutputs(0)
	{}

	/**
	 * \brief Constructor for state-less calculating nodes with generic pin names
	 * - operators in vector of operations (static const std::vector<Operation>
	 * operations) \param keyWord enum defined in enum class EOperatorType \param
	 * defaultLabel default string label in the workspace \param numberOfInputs
	 * \todo to be removed - known in compile time \param inputTypes as defined in
	 * std::vector<EValueType> \param numberOfOutputs \todo to be removed - known
	 * in compile time \param outputTypes as defined in std::vector<EValueType>
	 */
	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes)
	    : keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
	      numberOfOutputs(numberOfOutputs), outputTypes(outputTypes)
	{}

	/**
	 * \brief Constructor for Value nodes (float, vec3, vec4, mat, quat). \todo Q
	 * There is no contructor for pulse? \param isConstructor have true here
	 */
	Operation(const std::string& keyWord, const std::string& defaultLabel, bool isConstructor, int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes)
	    : keyWord(keyWord), defaultLabel(defaultLabel), isConstructor(isConstructor), numberOfInputs(numberOfInputs),
	      inputTypes(inputTypes), numberOfOutputs(numberOfOutputs), outputTypes(outputTypes)
	{}

	/**
	 * \brief Constructor for Transform matrices constructors
	 *  (state-less calculating nodes with explicitly defined input pin names)
	 * \param defaultTagText - used rarely for a hoover tag (mostly used with
	 * NO_TAG "" ) \param defaultInputNames vector of strings with predefined
	 * input names
	 */
	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
	          const std::vector<std::string>& defaultInputNames)
	    : keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
	      numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
	      defaultInputNames(defaultInputNames)
	{}
	/**
	 * \brief Constructor for a Cycle node with named inputs and outputs - as
	 * static const Operation g_CycleProperties \param defaultTagText  - used
	 * rarely for a hoover tag (mostly used with NO_TAG "" ) \param
	 * defaultInputNames vector of strings with predefined input names \param
	 * defaultOutputNames vector of strings with predefined output names
	 */
	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
	          const std::vector<std::string>& defaultInputNames, const std::vector<std::string>& defaultOutputNames)
	    : keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
	      numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
	      defaultInputNames(defaultInputNames), defaultOutputNames(defaultOutputNames)
	{}

	/**
	 * \brief Constructor for a Cycle node with named inputs and outputs - as
	 * static const Operation g_CycleProperties \param defaultTagText  - used
	 * rarely for a hoover tag (mostly used with NO_TAG "" ) \param
	 * defaultInputNames vector of strings with predefined input names
	 */
	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText)
	    : keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
	      numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText)
	{}

	// \todo NEVER USED
	// Operation(const std::string& keyWord, const std::string& defaultLabel,
	//					const std::vector<std::string>&	defaultInputNames) :
	//		keyWord(keyWord),
	//		defaultLabel(defaultLabel), defaultInputNames(defaultInputNames),
	// numberOfInputs(0), 		numberOfOutputs(0)
	//{}
};

//\todo - rename to EOperatorType instead of this:  using EOperatorType =
// EOperatorType;
enum class EOperatorType
{
	Inversion,
	Transpose,
	Determinant,
	MatrixMulMatrix,
	MatrixAddMatrix,           // done SS
	MatrixMulVector,           // done SS
	VectorMulMatrix,           // done SS
	MatrixMulFloat,            // done SS
	VectorDotVector,           // done SS
	VectorAddVector,           // done SS
	VectorSubVector,           // done SS
	VectorMulFloat,            // done SS
	VectorPerspectiveDivision, // done SS
	NormalizeVector,           // done JH
	MixVector,                 // done SS
	Vector3CrossVector3,       // done SS
	Vector3DotVector3,         // done SS
	Vector3AddVector3,         // done SS
	Vector3SubVector3,         // done SS
	Vector3MulFloat,           // done SS
	NormalizeVector3,          // done SS
	Vector3Length,             // done SS
	ShowVector3,               // done SS
	MixVector3,                // done SS
	ConjQuat,                  // done SS
	FloatVecToQuat,            // done SS
	AngleAxisToQuat,           // done SS
	VecVecToQuat,              // done SS
	QuatToFloatVec,            // done SS
	QuatToAngleAxis,           // done SS
	QuatToEuler,               // done SS
	EulerToQuat,               // done SS
	QuatInverse,               // done SS
	QuatSlerp,                 // done SS
	QuatLongWaySlerp,          // done SS
	QuatLerp,                  // done SS
	FloatMulQuat,              // done SS
	QuatMulQuat,               // done SS
	QuatVecConjQuat,           // done SS
	QuatLength,                // done SS
	ClampFloat,                // done SS
	FloatMulFloat,             // done SS
	FloatDivFloat,             // done SS
	FloatAddFloat,             // done SS
	FloatPowFloat,             // done SS
	MixFloat,                  // done SS
	FloatSinCos,               // done SS
	ASinACos,                  // done SS
	Signum,                    // done SS
	MatrixToVectors,           // done SS
	Vectors3ToMatrix,          // done SS
	VectorsToMatrix,           // done SS
	MatrixToFloats,            // done SS
	FloatsToMatrix,            // done SS
	MatrixToTR,                // done SS
	TRToMatrix,                // done SS
	MatrixToQuat,              // done SS
	QuatToMatrix,              // done SS
	VectorToFloats,            // done SS
	FloatsToVector,            // done SS
	Vector3ToFloats,           // done SS
	FloatsToVector3,           // done SS
	VectorToVector3,           // done SS
	Vector3ToVector,           // done SS
	QuatToFloats,              // done SS
	FloatsToQuat,              // done SS
	NormalizeQuat,             // done SS
	FloatToFloat,              // done SS
	Vector3ToVector3,          // done JH
	Vector4ToVector4,          // done JH
	MatrixToMatrix,            // done JH
	QuatToQuat,                // done SS

	// Transform matrices "constructors"
	MakeTranslation, // done SS
	MakeEulerX,      // done SS
	MakeEulerY,      // done SS
	MakeEulerZ,      // done SS
	MakeScale,       // done SS
	MakeAxisAngle,   // done SS
	MakeOrtho,       // done SS
	MakePerspective, // done SS
	MakeFrustum,     // done SS
	MakeLookAt,      // done SS

	Screen, // done SS
	Pulse   // done SS

	// SS trackball (trackcube) is done, waiting for Core
};

enum class ETransformType
{
	// This is for a sequence.
	Free = 0,    // 0  PREP-
	Translation, // 1  PREP-
	EulerX,      // 2  PREP-
	EulerY,      // 3  PREP-
	EulerZ,      // 4  PREP-
	Scale,       // 5  PREP-
	AxisAngle,   // 6  PREP-
	Quat,        // 7  PREP-
	Ortho,       // 8  PREP-
	Perspective, // 9  PREP-
	Frustum,     // 10 PREP-
	LookAt,      // 11 PREP-
};

// default string name of each OpValueType (defined in NodeData.h)
static const std::array<const char*, 8> defaultIoNames = {
    "pulse",  // PULSE		MN dodelat
    "float",  // Float
    "vec3",   // Vec3
    "vec4",   // Vec4
    "matrix", // Matrix
    "quat",   // Quat
    "",       // Matrix_MULL	MN dodelat
    ""        // SCREEN		MN dodelat
};

// variants of input and output pins
static const std::vector<EValueType> matrixInput = {EValueType::Matrix};
static const std::vector<EValueType> vectorInput = {EValueType::Vec4};
static const std::vector<EValueType> vector3Input = {EValueType::Vec3};
static const std::vector<EValueType> floatInput = {EValueType::Float};
static const std::vector<EValueType> quatInput = {EValueType::Quat};

static const std::vector<EValueType> twoMatrixInput = {EValueType::Matrix, EValueType::Matrix};
static const std::vector<EValueType> twoVectorInput = {EValueType::Vec4, EValueType::Vec4};
static const std::vector<EValueType> twoVector3Input = {EValueType::Vec3, EValueType::Vec3};
static const std::vector<EValueType> twoFloatInput = {EValueType::Float, EValueType::Float};
static const std::vector<EValueType> twoQuatInput = {EValueType::Quat, EValueType::Quat};

static const std::vector<EValueType> threeVector3Input = {EValueType::Vec3, EValueType::Vec3, EValueType::Vec3};
static const std::vector<EValueType> threeFloatInput = {EValueType::Float, EValueType::Float, EValueType::Float};

static const std::vector<EValueType> fourVectorInput = {EValueType::Vec4, EValueType::Vec4, EValueType::Vec4,
                                                        EValueType::Vec4};
static const std::vector<EValueType> fourVector3Input = {EValueType::Vec3, EValueType::Vec3, EValueType::Vec3,
                                                         EValueType::Vec3};
static const std::vector<EValueType> fourFloatInput = {EValueType::Float, EValueType::Float, EValueType::Float,
                                                       EValueType::Float};

static const std::vector<EValueType> sixFloatInput = {EValueType::Float, EValueType::Float, EValueType::Float,
                                                      EValueType::Float, EValueType::Float, EValueType::Float};

static const std::vector<EValueType> sixteenFloatInput = {
    EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float,
    EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float,
    EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float};

static const std::vector<EValueType> twoVectorFloatInput = {EValueType::Vec4, EValueType::Vec4, EValueType::Float};
static const std::vector<EValueType> twoVector3FloatInput = {EValueType::Vec3, EValueType::Vec3, EValueType::Float};
static const std::vector<EValueType> twoFloatVector3Input = {EValueType::Float, EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> twoQuatFloatInput = {EValueType::Quat, EValueType::Quat, EValueType::Float};

static const std::vector<EValueType> matrixVectorInput = {EValueType::Matrix, EValueType::Vec4};
static const std::vector<EValueType> vectorMatrixInput = {EValueType::Vec4, EValueType::Matrix};
static const std::vector<EValueType> vector3FloatInput = {EValueType::Vec3, EValueType::Float};
static const std::vector<EValueType> floatMatrixInput = {EValueType::Float, EValueType::Matrix};
static const std::vector<EValueType> floatVectorInput = {EValueType::Float,
                                                         EValueType::Vec4}; // PF: vec3 changed to vec4
static const std::vector<EValueType> floatVector3Input = {EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> floatQuatInput = {EValueType::Float, EValueType::Quat};
static const std::vector<EValueType> quatVector3Input = {EValueType::Quat, EValueType::Vec3};

static const std::vector<EValueType> matrixMulAndMatrixInput = {EValueType::MatrixMul, EValueType::Matrix};
static const std::vector<EValueType> matrixMulInput = {EValueType::MatrixMul};

// explicitly defined pin names
static const std::vector<std::string> mixInputNames = {"from", "to", "t"};
static const std::vector<std::string> AngleAxisToQuatInputNames = {"angle", "angle / 2", "vec3"};
static const std::vector<std::string> QuatToFloatVecInputNames = {"w", "(x,y,z)"};
static const std::vector<std::string> AngleAxisInputNames = {"angle", "axis"};
static const std::vector<std::string> Vectors3ToMatrixInputNames = {"vec3 X", "vec3 Y", "vec3 Z", "vec3 T"};
static const std::vector<std::string> VectorsToMatrixInputNames = {"vec4 X", "vec4 Y", "vec4 Z", "vec4 T"};
static const std::vector<std::string> ClampFloatInputNames = {"val", "min", "max"};
static const std::vector<std::string> xyz = {"x", "y", "z"};
static const std::vector<std::string> xyzw = {"x", "y", "z", "w"};
static const std::vector<std::string> tr = {"T", "R"};
static const std::vector<std::string> eulerInputNames = {"angle"};
static const std::vector<std::string> orthoFrustrumInputNames = {"left", "right", "bottom", "top", "near", "far"};
static const std::vector<std::string> PerspectiveInputNames = {"fovy", "aspect", "near", "far"};
static const std::vector<std::string> lookAtInputNames = {"eye", "center", "up"};

/**
 * \brief Table with configuration parameters for OPERATORS
 *
 * \note Warning: C6262: Function uses '23120' bytes of stack> exceeds
 * /analyze>stacksize '16384'.. This allocation was for a compiler-generated
 * temporary for 'struct Operation [85]' at line 350. Consider moving some data
 * to heap.
 */
static const std::vector<Operation> operations = {
    {n(EOperatorType::Inversion), "inversion", 1, matrixInput, 1, matrixInput},          // inversion
    {n(EOperatorType::Transpose), "transpose", 1, matrixInput, 1, matrixInput},          // transpose
    {n(EOperatorType::Determinant), "determinant", 1, matrixInput, 1, floatInput},       // determinant
    {n(EOperatorType::MatrixMulMatrix), "mat * mat", 2, twoMatrixInput, 1, matrixInput}, // mat * mat
    {n(EOperatorType::MatrixAddMatrix), "mat + mat", 2, twoMatrixInput, 1, matrixInput}, // mat + mat
    {n(EOperatorType::MatrixMulVector), "mat * vec4", 2, matrixVectorInput, 1,
     vectorInput}, // mat * vec4 (M * column vector)
    {n(EOperatorType::VectorMulMatrix), "vec4 * mat", 2, vectorMatrixInput, 1,
     vectorInput}, // vec4 * mat	 (row vector * M)
    {n(EOperatorType::MatrixMulFloat), "float * mat", 2, floatMatrixInput, 1, matrixInput},  // float * mat
    {n(EOperatorType::VectorDotVector), "vec4 . vec4", 2, twoVectorInput, 1, floatInput},    // vec4 . vec4
    {n(EOperatorType::VectorAddVector), "vec4 + vec4", 2, twoVectorInput, 1, vectorInput},   // vec4 + vec4
    {n(EOperatorType::VectorSubVector), "vec4 - vec4", 2, twoVectorInput, 1, vectorInput},   // vec4 - vec4
    {n(EOperatorType::VectorMulFloat), "float * vec4", 2, floatVectorInput, 1, vectorInput}, // float * vec4
    {n(EOperatorType::VectorPerspectiveDivision), "perspective division", 1, vectorInput, 1,
     vectorInput},                                                                         // perspective division
    {n(EOperatorType::NormalizeVector), "normalize vec4", 1, vectorInput, 1, vectorInput}, // normalize vec4
    {n(EOperatorType::MixVector), "mix vec4", 3, twoVectorFloatInput, 1, vectorInput, NO_TAG,
     mixInputNames},                                                                             // mix vec4
    {n(EOperatorType::Vector3CrossVector3), "vec3 x vec3", 2, twoVector3Input, 1, vector3Input}, // vec3 x vec3
    {n(EOperatorType::Vector3DotVector3), "vec3 . vec3", 2, twoVector3Input, 1, floatInput},     // vec3 . vec3
    {n(EOperatorType::Vector3AddVector3), "vec3 + vec3", 2, twoVector3Input, 1, vector3Input},   // vec3 + vec3
    {n(EOperatorType::Vector3SubVector3), "vec3 - vec3", 2, twoVector3Input, 1, vector3Input},   // vec3 - vec3
    {n(EOperatorType::Vector3MulFloat), "float * vec3", 2, floatVector3Input, 1, vector3Input},  // float * vec3
    {n(EOperatorType::NormalizeVector3), "normalize vec3", 1, vector3Input, 1, vector3Input},    // normalize vec3
    {n(EOperatorType::Vector3Length), "lengthvec3", 1, vector3Input, 1, floatInput,
     "l = sqrt(x*x + y*y + z*z)"}, // lengthvec3
    {n(EOperatorType::ShowVector3), "show vec3", 1, vector3Input, 1, matrixInput,
     "Create the matrix rotating vector [1, 0, 0] to the input vector. Should "
     "be used for display of rotated "
     "vectors."}, // show vec3
    {n(EOperatorType::MixVector3), "mix vec3", 3, twoVector3FloatInput, 1, vector3Input, NO_TAG,
     mixInputNames},                                                                           // mix vec3
    {n(EOperatorType::ConjQuat), "quat conjugate", 1, quatInput, 1, quatInput},                // quat conjugate
    {n(EOperatorType::FloatVecToQuat), "quatfloat, vec3", 2, floatVector3Input, 1, quatInput}, // quatfloat, vec3
    {n(EOperatorType::AngleAxisToQuat), "quatangle, axis", 3, twoFloatVector3Input, 1, quatInput, NO_TAG,
     AngleAxisToQuatInputNames}, // quatangle, axis
    {n(EOperatorType::VecVecToQuat), "quatvec3, vec3", 2, twoVector3Input, 1, quatInput,
     "Input vector's are normalized."}, // quatvec3, vec3   //PF: floatInput
                                        // changed to quatInput
    {n(EOperatorType::QuatToFloatVec), "quat -> float, vec3", 1, quatInput, 2, floatVector3Input, NO_TAG, DEFAULT_NAMES,
     QuatToFloatVecInputNames}, // quat -> float, vec3
    {n(EOperatorType::QuatToAngleAxis), "quat -> angle, axis", 1, quatInput, 2, floatVector3Input, NO_TAG,
     DEFAULT_NAMES, AngleAxisInputNames},                                               // quat -> angle, axis
    {n(EOperatorType::QuatToEuler), "quat -> euler", 1, quatInput, 3, threeFloatInput}, // quat -> euler
    {n(EOperatorType::EulerToQuat),
     "euler -> quat",
     3,
     threeFloatInput,
     1,
     quatInput,
     NO_TAG,
     {"x", "y", "z"}},                                                               // euler
                                                                                     // ->
                                                                                     // quat
    {n(EOperatorType::QuatInverse), "inverse quat", 1, quatInput, 1, quatInput},     // inverse quat
    {n(EOperatorType::QuatSlerp), "quat slerp", 3, twoQuatFloatInput, 1, quatInput}, // quat slerp
    {n(EOperatorType::QuatLongWaySlerp), "quat long way slerp", 3, twoQuatFloatInput, 1,
     quatInput},                                                                       // quat long way slerp
    {n(EOperatorType::QuatLerp), "quat lerp", 3, twoQuatFloatInput, 1, quatInput},     // quat lerp
    {n(EOperatorType::FloatMulQuat), "float * quat", 2, floatQuatInput, 1, quatInput}, // float * quat
    {n(EOperatorType::QuatMulQuat), "quat * quat", 2, twoQuatInput, 1, quatInput},     // quat * quat
    {n(EOperatorType::QuatVecConjQuat), "qvq*", 2, quatVector3Input, 1, vector3Input}, // qvq*
    {n(EOperatorType::QuatLength), "lengthquat", 1, quatInput, 1, floatInput,
     "l = sqrt(w*w + x*x + y*y + z*z)"}, // lengthquat
    {n(EOperatorType::ClampFloat), "clamp float", 3, threeFloatInput, 1, floatInput,
     "Clamp val to <min, max> interval.\n Default min = 0 and max = 1", ClampFloatInputNames}, // clamp float
    {n(EOperatorType::FloatMulFloat), "float * float", 2, twoFloatInput, 1, floatInput},       // float * float
    {n(EOperatorType::FloatDivFloat), "float / float", 2, twoFloatInput, 1, floatInput},       // float / float
    {n(EOperatorType::FloatAddFloat), "float + float", 2, twoFloatInput, 1, floatInput},       // float + float
    {n(EOperatorType::FloatPowFloat), "float ^ float", 2, twoFloatInput, 1, floatInput},       // float ^ float
    {n(EOperatorType::MixFloat), "mix float", 3, threeFloatInput, 1, floatInput, NO_TAG, mixInputNames}, // mix float
    {n(EOperatorType::FloatSinCos), "sin & cos(float)", 1, floatInput, 2, twoFloatInput},    // sin&cos(float)
    {n(EOperatorType::ASinACos), "asin & acos(float)", 1, floatInput, 2, twoFloatInput},     // asin&acos(float)
    {n(EOperatorType::Signum), "signum", 1, floatInput, 1, floatInput},                      // signum
    {n(EOperatorType::MatrixToVectors), "mat -> vecs4", 1, matrixInput, 4, fourVectorInput}, // mat -> vecs
    {n(EOperatorType::Vectors3ToMatrix), "vecs3 -> mat", 4, fourVector3Input, 1, matrixInput, NO_TAG,
     Vectors3ToMatrixInputNames}, // vecs3 -> mat
    {n(EOperatorType::VectorsToMatrix), "vecs4 -> mat", 4, fourVectorInput, 1, matrixInput, NO_TAG,
     VectorsToMatrixInputNames}, // vecs4 -> mat
    {n(EOperatorType::MatrixToFloats), "mat -> floats", 1, matrixInput, 16, sixteenFloatInput, NO_TAG, DEFAULT_NAMES,
     matrixIndexNames()}, // mat -> floats
    {n(EOperatorType::FloatsToMatrix), "floats -> mat", 16, sixteenFloatInput, 1, matrixInput, NO_TAG,
     matrixIndexNames()}, // floats -> mat
    {n(EOperatorType::MatrixToTR), "mat -> TR", 1, matrixInput, 2, twoMatrixInput, NO_TAG, DEFAULT_NAMES,
     tr},                                                                                       // mat -> TR
    {n(EOperatorType::TRToMatrix), "TR -> mat", 2, twoMatrixInput, 1, matrixInput, NO_TAG, tr}, // TR -> mat
    {n(EOperatorType::MatrixToQuat), "mat -> quat", 1, matrixInput, 1, quatInput},              // mat -> quat
    {n(EOperatorType::QuatToMatrix), "quat -> mat", 1, quatInput, 1, matrixInput},              // quat -> mat
    {n(EOperatorType::VectorToFloats), "vec4 -> floats", 1, vectorInput, 4, fourFloatInput, NO_TAG, DEFAULT_NAMES,
     xyzw}, // vec4 -> float
    {n(EOperatorType::FloatsToVector), "floats -> vec4", 4, fourFloatInput, 1, vectorInput, NO_TAG,
     xyzw}, // floats -> vec4
    {n(EOperatorType::Vector3ToFloats), "vec3 -> floats", 1, vector3Input, 3, threeFloatInput, NO_TAG, DEFAULT_NAMES,
     xyz}, // vec3 -> floats
    {n(EOperatorType::FloatsToVector3), "floats -> vec3", 3, threeFloatInput, 1, vector3Input, NO_TAG,
     xyz},                                                                                // floats -> vec3
    {n(EOperatorType::VectorToVector3), "vec4 -> vec3", 1, vectorInput, 1, vector3Input}, // vec4 -> vec3
    //
    {n(EOperatorType::Vector3ToVector), "vec3 -> vec4", 2, vector3FloatInput, 1, vectorInput}, // vec3 -> vec4
    {n(EOperatorType::QuatToFloats), "quat -> floats", 1, quatInput, 4, fourFloatInput, NO_TAG, DEFAULT_NAMES,
     xyzw},                                                                                            // quat -> floats
    {n(EOperatorType::FloatsToQuat), "floats -> quat", 4, fourFloatInput, 1, quatInput, NO_TAG, xyzw}, // floats -> quat
    {n(EOperatorType::NormalizeQuat), "normalize quat", 1, quatInput, 1, quatInput},                   // normalize quat

    // Value nodes.
    {n(EOperatorType::FloatToFloat), "float", true, 1, floatInput, 1, floatInput},
    {n(EOperatorType::Vector3ToVector3), "vec3", true, 1, vector3Input, 1, vector3Input},
    {n(EOperatorType::Vector4ToVector4), "vec4", true, 1, vectorInput, 1, vectorInput},
    {n(EOperatorType::MatrixToMatrix), "mat", true, 1, matrixInput, 1, matrixInput},
    {n(EOperatorType::QuatToQuat), "quat", true, 1, {EValueType::Quat}, 1, {EValueType::Quat}},

    // Transform matrices constructors
    // PF: I have deleted the world constructor from all operators
    {n(EOperatorType::MakeTranslation), "translate", 1, vector3Input, 1, matrixInput}, // translate
    {n(EOperatorType::MakeEulerX), "eulerAngleX", 1, floatInput, 1, matrixInput, NO_TAG,
     eulerInputNames}, // eulerAngleX
    {n(EOperatorType::MakeEulerY), "eulerAngleY", 1, floatInput, 1, matrixInput, NO_TAG,
     eulerInputNames}, // eulerAngleY
    {n(EOperatorType::MakeEulerZ), "eulerAngleZ", 1, floatInput, 1, matrixInput, NO_TAG,
     eulerInputNames},                                                       // eulerAngleZ
    {n(EOperatorType::MakeScale), "scale", 1, vector3Input, 1, matrixInput}, // scale
    {n(EOperatorType::MakeAxisAngle), "rotate", 2, floatVector3Input, 1, matrixInput, NO_TAG,
     AngleAxisInputNames},                                                                                     // rotate
    {n(EOperatorType::MakeOrtho), "ortho", 6, sixFloatInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames}, // ortho
    {n(EOperatorType::MakePerspective), "perspective", 4, fourFloatInput, 1, matrixInput, NO_TAG,
     PerspectiveInputNames}, // perspective
    {n(EOperatorType::MakeFrustum), "frustum", 6, sixFloatInput, 1, matrixInput, NO_TAG,
     orthoFrustrumInputNames}, // frustrum
    {n(EOperatorType::MakeLookAt), "lookAt", 3, threeVector3Input, 1, matrixInput, NO_TAG, lookAtInputNames}, // lookAt

    {n(EOperatorType::Screen), "screen", 1, {EValueType::Screen}, 2, {EValueType::Screen, EValueType::Float}},
    {n(EOperatorType::Pulse), "pulse", 0, {}, 1, {EValueType::Pulse}}};

/**
 * From, to, multiplier, receive (play, pause, stopAndReset, prev, next).
 */
static const PinGroup cycleInputs = {
    EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Pulse,
    EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse,
};

/**
 * Output value, emit (play, pause, stopAndReset, prev, next).
 */
static const PinGroup cycleOutputs = {
    EValueType::Float, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse,
    EValueType::Pulse, EValueType::Pulse, EValueType::Pulse,
};

// clang-format off
static const Operation g_CycleProperties = {
	"Cycle", "cycle",
    8, cycleInputs,
	7, cycleOutputs,
    NO_TAG,
	{"from", "to", "step", "play", "pause", "stop", "prev", "next"},
	{"value", "play", "pause", "stop", "prev", "next", "end"}
};
// clang-format on

inline const Operation g_sequence = {
    "Sequence", "seq", 2, matrixMulAndMatrixInput, 3, {EValueType::MatrixMul, EValueType::Matrix, EValueType::Matrix}};

inline const Operation g_cameraProperties = {
    "Camera", "camera", 0, {}, 3, {EValueType::Screen, EValueType::Matrix, EValueType::MatrixMul}};

inline static const Operation g_modelProperties = {
    "Model", "model", 1, matrixMulInput, 0, {},
};

FORCE_INLINE const Operation* getOperationProps(EOperatorType type)
{
	return &operations[static_cast<size_t>(type)];
}

//===-- TRANSFORMS --------------------------------------------------------===//

/**
 * \brief ROW ORDER flags, if the mat4x4 values are editable (i.e., each single
 * value may be changed by the mouse drag or not)
 */
using TransformMask = std::bitset<16>;

/**
 * \brief Extends Operation - adds properties to transform nodes
 */
struct TransformOperation
{
	struct NameTypePair
	{
		std::string name;
		EValueType type;
	};

	bool hasDefaultValue(const std::string& name) const;

	std::optional<NameTypePair> getDefaultValueType(const std::string& name) const;

	/// Names and types of DefaultValues (parameters for matrix definition in the
	/// middle LOD, such as fovy in Perspective)
	using ValueMap = std::vector<NameTypePair>;

	Operation operation;
	TransformMask mask;
	ValueMap defaultValuesTypes;
	bool isRotation = false;
};

constexpr TransformMask g_AllLocked = 0b0000000000000000;
constexpr TransformMask g_AllUnlocked = 0b1111111111111111;

/// All entries must be in the same order as ETransformType enum entries.
// clang-format off
static inline const std::vector<TransformOperation> g_transforms = {
    {{n(ETransformType::Free), "free"}, g_AllUnlocked, {}},
    {{n(ETransformType::Translation), "translate"},
     0b0001000100010000, // the last column
     {{"translation", EValueType::Vec3}}},
    {
        {n(ETransformType::EulerX), "eulerAngleX"},
        0b0000011001100000,
        {{"rotation", EValueType::Float}},
        true
    },
    {
        {n(ETransformType::EulerY), "eulerAngleY"},
        0b1010000010100000,
        {{"rotation", EValueType::Float}},
        true
    },
    {
        {n(ETransformType::EulerZ), "eulerAngleZ"},
        0b1100110000000000,
        {{"rotation", EValueType::Float}},
        true
    },
    {
        {n(ETransformType::Scale), "scale"},
     0b1000010000100000, // the diagonal
     {{"scale", EValueType::Vec3}}
    },
    {
        {n(ETransformType::AxisAngle), "rotate"},
        0b1110111011100000,
        {
            {"axis", EValueType::Vec3},
            {"rotation", EValueType::Float}
        },
        true
    },
    {
        {n(ETransformType::Quat), "quat"},
        g_AllLocked,
        {
            {"quat", EValueType::Quat}
        },
        true
    },
    {{n(ETransformType::Ortho), "ortho"},
     0b1001010100110000,
     {{"left", EValueType::Float},
      {"right", EValueType::Float},
      {"bottom", EValueType::Float},
      {"top", EValueType::Float},
      {"near", EValueType::Float},
      {"far", EValueType::Float}}},
    {{n(ETransformType::Perspective), "perspective"},
     // 0b1000010000110010,
     0b1000010000110000, // forbid to edit -1 in the last row
     {{"fovy", EValueType::Float},
      {"aspect", EValueType::Float},
      {"near", EValueType::Float},
      {"far", EValueType::Float}}},
    {{n(ETransformType::Frustum), "frustum"},
     // 0b1010011000110010,       // forbid to edit -1 in the last row
     0b1010011000110000,
     {{"left", EValueType::Float},
      {"right", EValueType::Float},
      {"bottom", EValueType::Float},
      {"top", EValueType::Float},
      {"near", EValueType::Float},
      {"far", EValueType::Float}}},
    {{n(ETransformType::LookAt), "lookAt"},
     g_AllLocked,
     {{"eye", EValueType::Vec3}, {"center", EValueType::Vec3}, {"up", EValueType::Vec3}}},
};
// clang-format on

FORCE_INLINE const TransformOperation& getTransformOperation(ETransformType type)
{
	return g_transforms[static_cast<size_t>(type)];
}

std::optional<TransformOperation*> getTransformOperation(const std::string& keyWord);

FORCE_INLINE const TransformOperation::ValueMap& getTransformDefaults(const std::string& keyWord)
{
	static TransformOperation::ValueMap noDefaults;

	auto type = magic_enum::enum_cast<ETransformType>(keyWord);
	if (type.has_value())
		return getTransformOperation(*type).defaultValuesTypes;
	return noDefaults;
}

FORCE_INLINE const TransformMask& getTransformMap(const std::string& keyWord)
{
	static TransformMask noDefaults = 0b1111111111111111;

	auto type = magic_enum::enum_cast<ETransformType>(keyWord);
	if (type.has_value())
		return getTransformOperation(*type).mask;
	return noDefaults;
}

FORCE_INLINE const TransformMask& getTransformMap(ETransformType transformType)
{
	return getTransformOperation(transformType).mask;
}

FORCE_INLINE const auto& getTransformDefaults(ETransformType type)
{
	return getTransformDefaults(n(type));
}

const TransformOperation& getTransformProps(const Operation* op);
}; // namespace Core

/**
 * File with operations table.
 */
#pragma once

#include <sstream>

#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/transform.hpp"

#include "Core/Defs.h"
#include "Core/Nodes/Node.h"

#include "NodeData.h"

static const std::vector<std::string> emptyNames = {};

#define NO_TAG        ""
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

namespace Core
{
using ValidDataMaps = std::vector<const Transform::DataMap*>;

static const ValidDataMaps defaultDataMaps = { &Transform::g_AllLocked, &Transform::g_Free };
}

using PinGroup = std::vector<EValueType>;

struct Operation
{
	using Pins = std::vector<EValueType>;

	const std::string keyWord;
	const std::string defaultLabel;
	const int numberOfInputs;
	const std::vector<EValueType> inputTypes;
	const int numberOfOutputs;
	const std::vector<EValueType> outputTypes;
	const std::string defaultTagText = NO_TAG;
	const std::vector<std::string> defaultInputNames =
			DEFAULT_NAMES; // if the names are not the names of the OpValueType
	const std::vector<std::string> defaultOutputNames =
			DEFAULT_NAMES; // if the names are not the names of the OpValueType
	const std::vector<const Core::Transform::DataMap*> validDatamaps = { &Core::Transform::g_AllLocked };



	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes)
			: keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
				numberOfOutputs(numberOfOutputs), outputTypes(outputTypes)
	{
	}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
	          const std::vector<std::string>& defaultInputNames,
	          const std::vector<const Core::Transform::DataMap*>& validDatamaps)
			: keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
				numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
				defaultInputNames(defaultInputNames), validDatamaps(validDatamaps)
	{
	}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes,
	          const std::vector<const Core::Transform::DataMap*>& validDatamaps)
			: keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
				numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), validDatamaps(validDatamaps)
	{
	}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
	          const std::vector<std::string>& defaultInputNames, const std::vector<std::string>& defaultOutputNames)
			: keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
				numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
				defaultInputNames(defaultInputNames), defaultOutputNames(defaultOutputNames)
	{
	}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
	          const std::vector<std::string>& defaultInputNames)
			: keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
				numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
				defaultInputNames(defaultInputNames)
	{
	}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
	          const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
	          const std::vector<EValueType>& outputTypes, const std::string& defaultTagText)
			: keyWord(keyWord), defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
				numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText)
	{
	}
};

enum class ENodeType
{
	Inversion,	//done SS
	Transpose,	//done SS
	Determinant, //done SS
	MatrixMulMatrix, //done SS
	MatrixAddMatrix,	//done SS
	MatrixMulVector,	//done SS
	VectorMulMatrix,	//done SS
	MatrixMulFloat,		//done SS
	VectorDotVector,	//done SS
	VectorAddVector,	//done SS
	VectorSubVector,	//done SS
	VectorMulFloat,	//done SS
	VectorPerspectiveDivision,//done SS
	NormalizeVector, //done JH
	MixVector,//done SS
	Vector3CrossVector3,	//done SS
	Vector3DotVector3,	//done SS
	Vector3AddVector3,	//done SS
	Vector3SubVector3,	//done SS
	Vector3MulFloat,	//done SS
	NormalizeVector3,	//done SS
	Vector3Length,	//done SS
	ShowVector3,	//done SS
	MixVector3,	//done SS
	ConjQuat,
	FloatVecToQuat,
	AngleAxisToQuat,
	VecVecToQuat,
	QuatToFloatVec,
	QuatToAngleAxis,
	QuatToEuler,
	EulerToQuat,
	QuatInverse,
	QuatSlerp,
	QuatLongWaySlerp,
	QuatLerp,
	FloatMulQuat,
	QuatMulQuat,
	QuatVecConjQuat,
	QuatLength,
	ClampFloat,
	FloatMulFloat,
	FloatDivFloat,
	FloatAddFloat,
	FloatPowFloat,
	MixFloat,
	FloatSinCos,
	ASinACos,
	Signum,
	MatrixToVectors,
	Vectors3ToMatrix,
	VectorsToMatrix,
	MatrixToFloats,
	FloatsToMatrix,
	MatrixToTR,
	TRToMatrix,
	MatrixToQuat,
	QuatToMatrix,
	VectorToFloats,
	FloatsToVector,
	Vector3ToFloats,
	FloatsToVector3,
	VectorToVector3,
	Vector3ToVector,
	QuatToFloats,
	FloatsToQuat,
	NormalizeQuat,
	Float, //done SS
	Vector3, //done JH
	Vector4, //done JH
	Matrix, //done JH
	Model,

	// Transform matrices "constructors"
	MakeTranslation, //done JH
	MakeEulerX, //done SS
	MakeEulerY, //done SS
	MakeEulerZ, //done SS
	MakeScale, //done JH
	MakeAxisAngle, //done SS
	MakeOrtho, //done SS
	MakePerspective, //done SS
	MakeFrustum, //done SS
	MakeLookAt, //done SS

	Screen,
	Pulse
};

enum class ETransformType
{
	//This is for sequence
	Free = 0,
	Translation, //done JH
	EulerX,
	EulerY,
	EulerZ,
	Scale, 
	AxisAngle,
	Quat,
	Ortho,
	Perspective,
	Frustum,
	LookAt,
};

// pro kazdy OpValueType (NodeData.h) je jeden string
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

static const std::vector<EValueType> matrixInput = {EValueType::Matrix};
static const std::vector<EValueType> vectorInput = {EValueType::Vec4};
static const std::vector<EValueType> vector3Input = {EValueType::Vec3};
static const std::vector<EValueType> floatInput = {EValueType::Float};
static const std::vector<EValueType> quatInput = {EValueType::Quat};

static const std::vector<EValueType> twoMatrixInput = {EValueType::Matrix, EValueType::Matrix};
static const std::vector<EValueType> twoVectorInput = {EValueType::Vec4, EValueType::Vec4};
static const std::vector<EValueType> twoVector3Input = {EValueType::Vec3, EValueType::Vec3};
static const std::vector<EValueType> twoQuatInput = {EValueType::Quat, EValueType::Quat};
static const std::vector<EValueType> twoFloatInput = {EValueType::Float, EValueType::Float};

static const std::vector<EValueType> threeFloatInput = {EValueType::Float, EValueType::Float, EValueType::Float};
static const std::vector<EValueType> threeVector3Input = {EValueType::Vec3, EValueType::Vec3, EValueType::Vec3};

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
static const std::vector<EValueType> floatMatrixInput = {EValueType::Float, EValueType::Matrix};
static const std::vector<EValueType> floatVectorInput = {EValueType::Float, EValueType::Vec4};
static const std::vector<EValueType> floatVector3Input = {EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> floatQuatInput = {EValueType::Float, EValueType::Quat};
static const std::vector<EValueType> quatVector3Input = {EValueType::Quat, EValueType::Vec3};

static const std::vector<EValueType> matrixMulAndMatrixInput = {EValueType::MatrixMul, EValueType::Matrix};
static const std::vector<EValueType> matrixMulInput = {EValueType::MatrixMul};

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
static const std::vector<std::string> orthoFrustrumInputNames = {"left", "right", "bottom", "top", "zNear", "zFar"};
static const std::vector<std::string> PerspectiveInputNames = {"fovy", "aspect", "zNear", "zFar"};
static const std::vector<std::string> lookAtInputNames = {"eye", "center", "up"};

/**
 * \brief Table box with configuration parameters
 * \nevim VectorMulMatrix -  vec4 * mat	MN jaky je duvod teto operace a nema byt vysledek matice???
 */
static const std::vector<Operation> operations = {
		{"Inversion", "inversion", 1, matrixInput, 1, matrixInput},              // inversion
		{"Transpose", "transpose", 1, matrixInput, 1, matrixInput},              // transpose
		{"Determinant", "determinant", 1, matrixInput, 1, floatInput},           // determinant
		{"MatrixMulMatrix", "mat * mat", 2, twoMatrixInput, 1, matrixInput},     // mat * mat
		{"MatrixAddMatrix", "mat + mat", 2, twoMatrixInput, 1, matrixInput},     // mat + mat
		{"MatrixMulVector", "mat * vec4", 2, matrixVectorInput, 1, vectorInput}, // mat * vec4
		{"VectorMulMatrix", "vec4 * mat", 2, vectorMatrixInput, 1,
     vectorInput}, // vec4 * mat	MN jaky je duvod teto operace a nema byt vysledek matice???
		{"MatrixMulFloat", "float * mat", 2, floatMatrixInput, 1, matrixInput},                   // float * mat
		{"VectorDotVector", "vec4 . vec4", 2, twoVectorInput, 1, floatInput},                     // vec4 . vec4
		{"VectorAddVector", "vec4 + vec4", 2, twoVectorInput, 1, vectorInput},                    // vec4 + vec4
		{"VectorSubVector", "vec4 - vec4", 2, twoVectorInput, 1, vectorInput},                    // vec4 - vec4
		{"VectorMulFloat", "float * vec4", 2, floatVectorInput, 1, vectorInput},                  // float * vec4
		{"VectorPerspectiveDivision", "perspective division", 1, vectorInput, 1, vectorInput},    // perspective division
		{"NormalizeVector", "normalize vec4", 1, vectorInput, 1, vectorInput},                    // normalize vec4
		{"MixVector", "mix vec4", 3, twoVectorFloatInput, 1, vectorInput, NO_TAG, mixInputNames}, // mix vec4
		{"Vector3CrossVector3", "vec3 x vec3", 2, twoVector3Input, 1, vector3Input},              // vec3 x vec3
		{"Vector3DotVector3", "vec3 . vec3", 2, twoVector3Input, 1, floatInput},                  // vec3 . vec3
		{"Vector3AddVector3", "vec3 + vec3", 2, twoVector3Input, 1, vector3Input},                // vec3 + vec3
		{"Vector3SubVector3", "vec3 - vec3", 2, twoVector3Input, 1, vector3Input},                // vec3 - vec3
		{"Vector3MulFloat", "float * vec3", 2, floatVectorInput, 1, vector3Input},                // float * vec3
		{"NormalizeVector3", "normalize vec3", 1, vector3Input, 1, vector3Input},                 // normalize vec3
		{"Vector3Length", "lengthvec3", 1, vector3Input, 1, floatInput, "l = sqrt(x*x + y*y + z*z)"}, // lengthvec3
		{"ShowVector3", "show vec3", 1, vector3Input, 1, matrixInput,
     "Create the matrix transforming vector [1, 0, 0] to the input vector. Should be used with bind objects / basic "
     "/ vectors."},                                                                              // show vec3
		{"MixVector3", "mix vec3", 3, twoVector3FloatInput, 1, vector3Input, NO_TAG, mixInputNames}, // mix vec3
		{"ConjQuat", "quat conjugate", 1, quatInput, 1, quatInput},                                  // quat conjugate
		{"FloatVecToQuat", "quatfloat, vec3", 2, floatVector3Input, 1, quatInput},                   // quatfloat, vec3
		{"AngleAxisToQuat", "quatangle, axis", 3, twoFloatVector3Input, 1, quatInput, NO_TAG,
     AngleAxisToQuatInputNames}, // quatangle, axis
		{"VecVecToQuat", "quatvec3, vec3", 3, twoVector3Input, 1, floatInput,
     "Input vector's are normalized."}, // quatvec3, vec3
		{"QuatToFloatVec", "quat -> float, vec3", 1, quatInput, 2, floatVector3Input, NO_TAG, DEFAULT_NAMES,
     QuatToFloatVecInputNames}, // quat -> float, vec3
		{"QuatToAngleAxis", "quat -> angle, axis", 1, quatInput, 2, floatVector3Input, NO_TAG, DEFAULT_NAMES,
     AngleAxisInputNames},                                              // quat -> angle, axis
		{"QuatToEuler", "quat -> euler", 1, quatInput, 3, threeFloatInput}, // quat -> euler
		{"EulerToQuat", "euler -> quat", 3, threeFloatInput, 1, quatInput, NO_TAG, {"x", "y", "z"}}, // euler -> quat
		{"QuatInverse", "inverse quat", 1, quatInput, 1, quatInput},                                 // inverse quat
		{"QuatSlerp", "quat slerp", 3, twoQuatFloatInput, 1, quatInput},                             // quat slerp
		{"QuatLongWaySlerp", "quat long way slerp", 3, twoQuatFloatInput, 1, quatInput}, // quat long way slerp
		{"QuatLerp", "quat lerp", 3, twoQuatFloatInput, 1, quatInput},                   // quat lerp
		{"FloatMulQuat", "float * quat", 2, floatQuatInput, 1, quatInput},               // float * quat
		{"QuatMulQuat", "quat * quat", 2, twoQuatInput, 1, quatInput},                   // quat * quat
		{"QuatVecConjQuat", "qvq*", 2, quatVector3Input, 1, vector3Input},               // qvq*
		{"QuatLength", "lengthquat", 1, quatInput, 1, floatInput, "l = sqrt(w*w + x*x + y*y + z*z)"}, // lengthquat
		{"ClampFloat", "clamp float", 3, threeFloatInput, 1, floatInput,
     "Clamp val to <min, max> interval.\n Default min = 0 and max = 1", ClampFloatInputNames}, // clamp float
		{"FloatMulFloat", "float * float", 2, twoFloatInput, 1, floatInput},                       // float * float
		{"FloatDivFloat", "float / float", 2, twoFloatInput, 1, floatInput},                       // float / float
		{"FloatAddFloat", "float + float", 2, twoFloatInput, 1, floatInput},                       // float + float
		{"FloatPowFloat", "float ^ float", 2, twoFloatInput, 1, floatInput},                       // float ^ float
		{"MixFloat", "mix float", 3, threeFloatInput, 1, floatInput, NO_TAG, mixInputNames},       // mix float
		{"FloatSinCos", "sin&cosfloat", 1, floatInput, 2, twoFloatInput},                          // sin&cosfloat
		{"ASinACos", "asin&acosfloat", 1, floatInput, 2, twoFloatInput},                           // asin&acosfloat
		{"Signum", "signum", 1, floatInput, 1, floatInput},                                        // signum
		{"MatrixToVectors", "mat -> vecs", 1, matrixInput, 4, fourVectorInput},                    // mat -> vecs
		{"Vectors3ToMatrix", "vecs3 -> mat", 4, fourVector3Input, 1, matrixInput, NO_TAG,
     Vectors3ToMatrixInputNames}, // vecs3 -> mat
		{"VectorsToMatrix", "vecs4 -> mat", 4, fourVectorInput, 1, matrixInput, NO_TAG,
     VectorsToMatrixInputNames}, // vecs4 -> mat
		{"MatrixToFloats", "mat -> floats", 1, matrixInput, 16, sixteenFloatInput, NO_TAG, DEFAULT_NAMES,
     matrixIndexNames()}, // mat -> floats
		{"FloatsToMatrix", "floats -> mat", 16, sixteenFloatInput, 1, matrixInput, NO_TAG,
     matrixIndexNames()},                                                                      // floats -> mat
		{"MatrixToTR", "mat -> TR", 1, matrixInput, 2, twoMatrixInput, NO_TAG, DEFAULT_NAMES, tr}, // mat -> TR
		{"TRToMatrix", "TR -> mat", 2, twoMatrixInput, 1, matrixInput, NO_TAG, tr},                // TR -> mat
		{"MatrixToQuat", "mat -> quat", 1, matrixInput, 1, quatInput},                             // mat -> quat
		{"QuatToMatrix", "quat -> mat", 1, quatInput, 1, matrixInput},                             // quat -> mat
		{"VectorToFloats", "vec4 -> floats", 1, vectorInput, 4, fourFloatInput, NO_TAG, DEFAULT_NAMES,
     xyzw},                                                                                // vec4 -> float
		{"FloatsToVector", "floats -> vec4", 4, fourFloatInput, 1, vectorInput, NO_TAG, xyzw}, // floats -> vec4
		{"Vector3ToFloats", "vec3 -> floats", 1, vector3Input, 3, threeFloatInput, NO_TAG, DEFAULT_NAMES,
     xyz},                                                                                   // vec3 -> floats
		{"FloatsToVector3", "floats -> vec3", 3, threeFloatInput, 1, vector3Input, NO_TAG, xyz}, // floats -> vec3
		{"VectorToVector3", "vec4 -> vec3", 1, vectorInput, 1, vector3Input},                    // vec4 -> vec3
		{"Vector3ToVector", "vec3 -> vec4", 1, vector3Input, 1, vectorInput},                    // vec3 -> vec4
		{"QuatToFloats", "quat -> floats", 1, quatInput, 4, fourFloatInput, NO_TAG, DEFAULT_NAMES,
     xyzw},                                                                            // quat -> floats
		{"FloatsToQuat", "floats -> quat", 4, fourFloatInput, 1, quatInput, NO_TAG, xyzw}, // floats -> quat
		{"NormalizeQuat", "normalize quat", 1, quatInput, 1, quatInput},                   // normalize quat

		// Value nodes.
		{"FloatToFloat", "float", 1, floatInput, 1, floatInput, Core::defaultDataMaps},
		{"Vector3ToVector3", "vec3", 1, vector3Input, 1, vector3Input, Core::defaultDataMaps},
		{"Vector4ToVector4", "vec4", 1, vectorInput, 1, vectorInput, Core::defaultDataMaps},
		{"MatrixToMatrix", "mat", 1, matrixInput, 1, matrixInput, Core::defaultDataMaps},

		{"Model", "model", 1, matrixMulInput, 0, {}},

		// Transform matrices constructors
		{"MakeTranslation", "translate constructor", 1, vector3Input, 1, matrixInput},                     // translate
		{"MakeEulerX", "eulerAngleX constructor", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleX
		{"MakeEulerY", "eulerAngleY constructor", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleY
		{"MakeEulerZ", "eulerAngleZ constructor", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleZ
		{"MakeScale", "scale constructor", 1, vector3Input, 1, matrixInput},                               // scale
		{"MakeAxisAngle", "rotate constructor", 2, floatVector3Input, 1, matrixInput, NO_TAG,
     AngleAxisInputNames},                                                                                 // rotate
		{"MakeOrtho", "ortho constructor", 6, sixFloatInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames}, // ortho
		{"MakePerspective", "perspective constructor", 4, fourFloatInput, 1, matrixInput, NO_TAG,
     PerspectiveInputNames},																			// perspective
		{"MakeFrustum", "frustum constructor", 6, sixFloatInput, 1, matrixInput, NO_TAG,
     orthoFrustrumInputNames},                                                                            // frustrum
		{"MakeLookAt", "lookAt constructor", 3, threeVector3Input, 1, matrixInput, NO_TAG, lookAtInputNames}, // lookAt

    {"Screen", "screen", 1, {EValueType::Screen}, 1, {EValueType::Float}},
    {"Pulse", "pulse", 0, {}, 1, {EValueType::Pulse}}
};

namespace Core
{
/**
 * From, to, multiplier, receive (play, pause, stopAndReset, prev, next).
 */
static const PinGroup cycleInputs = { EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, };

/**
 * Output value, emit (play, pause, stopAndReset, prev, next).
 */
static const PinGroup cycleOutputs = { EValueType::Float, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, EValueType::Pulse, };

static const Operation g_CycleProperties = {"Cycle", "cycle", 8, cycleInputs, 7, cycleOutputs };

inline const Operation g_sequence = {"Sequence", "seq", 2, matrixMulAndMatrixInput, 3, {EValueType::MatrixMul, EValueType::Matrix, EValueType::Matrix}};

static const Operation g_cameraProperties = { "Camera", "camera", 0, {}, 3, {EValueType::Screen, EValueType::Matrix, EValueType::MatrixMul} };

static const std::vector<Operation> g_transforms = {
		{"Free", "free", 0, matrixInput, 1, matrixInput, defaultDataMaps },                                              // free
		{"Translation", "translate", 0, matrixInput, 1, matrixInput, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Translate } },                                 // translate
		{"EulerX", "eulerAngleX", 0, matrixInput, 1, matrixInput, NO_TAG, eulerInputNames, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_EulerX } },            // eulerAngleX
		{"EulerY", "eulerAngleY", 0, matrixInput, 1, matrixInput, NO_TAG, eulerInputNames, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_EulerY } },            // eulerAngleY
		{"EulerZ", "eulerAngleZ", 0, matrixInput, 1, matrixInput, NO_TAG, eulerInputNames, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_EulerZ } },            // eulerAngleZ
		{"Scale", "scale", 0, matrixInput, 1, matrixInput, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Scale, &Transform::g_UniformScale } },                                            // scale
		{"AxisAngle", "rotate", 0, matrixInput, 1, matrixInput, NO_TAG, AngleAxisInputNames, defaultDataMaps },          // rotate
		{"Quat", "quat", 0, matrixInput, 1, matrixInput, NO_TAG, AngleAxisInputNames, defaultDataMaps },                 // quat rotate
		{"Ortho", "ortho", 0, matrixInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Ortho } },           // ortho
		{"Perspective", "perspective", 0, matrixInput, 1, matrixInput, NO_TAG, PerspectiveInputNames, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Perspective } }, // perspective
		{"Frustum", "frustum", 0, matrixInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames, { &Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Frustum } },       // frustrum
		{"LookAt", "lookAt", 0, matrixInput, 1, matrixInput, NO_TAG, lookAtInputNames, defaultDataMaps },                // lookAt
};

FORCE_INLINE const Operation* getOperationProps(ENodeType type)
{
	return &operations[static_cast<size_t>(type)];
}

FORCE_INLINE const Operation* getTransformProps(ETransformType type)
{
	return &g_transforms[static_cast<unsigned>(type)];
}
}; // namespace Core

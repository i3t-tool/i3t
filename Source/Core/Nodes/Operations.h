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
	std::string keyWord;
	std::string defaultLabel;
	std::vector<EValueType> inputTypes;
	std::vector<EValueType> outputTypes;
	std::string defaultTagText = NO_TAG;
	std::vector<std::string> defaultInputNames = DEFAULT_NAMES; // if the names differ from the names of the OpValueType
	std::vector<std::string> defaultOutputNames =
	    DEFAULT_NAMES; // if the names differ from the names of the OpValueType

	bool isConstructor = false; //< this is a standard operator node - processes only the connected
	                            // pins and has no GUI  editable values

	bool ignoreCycleDetection = false;
};

enum class EOperatorType
{
	Inversion,
	Transpose,
	Determinant,
	MatrixMulMatrix,
	MatrixAddMatrix,
	MatrixMulVector,
	VectorMulMatrix,
	MatrixMulFloat,
	VectorDotVector,
	VectorAddVector,
	VectorSubVector,
	VectorMulFloat,
	VectorPerspectiveDivision,
	NormalizeVector,
	MixVector,
	Vector3CrossVector3,
	Vector3DotVector3,
	Vector3AddVector3,
	Vector3SubVector3,
	Vector3MulFloat,
	NormalizeVector3,
	Vector3Length,
	ShowVector3,
	MixVector3,
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
	FloatToFloat,
	Vector3ToVector3,
	Vector4ToVector4,
	MatrixToMatrix,
	QuatToQuat,

	// Transform matrices "constructors"
	MakeTranslation,
	MakeEulerX,
	MakeEulerY,
	MakeEulerZ,
	MakeScale,
	MakeAxisAngle,
	MakeOrtho,
	MakePerspective,
	MakeFrustum,
	MakeLookAt,

	Screen,
	Pulse

	// todo trackball (trackcube)
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
static const std::array<std::string, 8> defaultIoNames = {
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
static const std::vector<EValueType> vector3QuatInput = {EValueType::Vec3, EValueType::Quat};
static const std::vector<EValueType> floatMatrixInput = {EValueType::Float, EValueType::Matrix};
static const std::vector<EValueType> floatVectorInput = {EValueType::Float,
                                                         EValueType::Vec4}; // PF: vec3 changed to vec4
static const std::vector<EValueType> floatVector3Input = {EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> floatQuatInput = {EValueType::Float, EValueType::Quat};
// static const std::vector<EValueType> quatVector3Input = {EValueType::Quat, EValueType::Vec3};

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
 */
static const std::vector<Operation> operations = {
    {n(EOperatorType::Inversion), "inversion", matrixInput, matrixInput},              // inversion
    {n(EOperatorType::Transpose), "transpose", matrixInput, matrixInput},              // transpose
    {n(EOperatorType::Determinant), "determinant", matrixInput, floatInput},           // determinant
    {n(EOperatorType::MatrixMulMatrix), "mat * mat", twoMatrixInput, matrixInput},     // mat * mat
    {n(EOperatorType::MatrixAddMatrix), "mat + mat", twoMatrixInput, matrixInput},     // mat + mat
    {n(EOperatorType::MatrixMulVector), "mat * vec4", matrixVectorInput, vectorInput}, // mat * vec4 (M * column vector)
    {n(EOperatorType::VectorMulMatrix), "vec4 * mat", vectorMatrixInput,
     vectorInput},                                                                     // vec4 * mat	 (row vector * M)
    {n(EOperatorType::MatrixMulFloat), "float * mat", floatMatrixInput, matrixInput},  // float * mat
    {n(EOperatorType::VectorDotVector), "vec4 . vec4", twoVectorInput, floatInput},    // vec4 . vec4
    {n(EOperatorType::VectorAddVector), "vec4 + vec4", twoVectorInput, vectorInput},   // vec4 + vec4
    {n(EOperatorType::VectorSubVector), "vec4 - vec4", twoVectorInput, vectorInput},   // vec4 - vec4
    {n(EOperatorType::VectorMulFloat), "float * vec4", floatVectorInput, vectorInput}, // float * vec4
    {n(EOperatorType::VectorPerspectiveDivision), "perspective division", vectorInput,
     vectorInput},                                                                   // perspective division
    {n(EOperatorType::NormalizeVector), "normalize vec4", vectorInput, vectorInput}, // normalize vec4
    {n(EOperatorType::MixVector), "mix vec4", twoVectorFloatInput, vectorInput, NO_TAG, mixInputNames}, // mix vec4
    {n(EOperatorType::Vector3CrossVector3), "vec3 x vec3", twoVector3Input, vector3Input},              // vec3 x vec3
    {n(EOperatorType::Vector3DotVector3), "vec3 . vec3", twoVector3Input, floatInput},                  // vec3 . vec3
    {n(EOperatorType::Vector3AddVector3), "vec3 + vec3", twoVector3Input, vector3Input},                // vec3 + vec3
    {n(EOperatorType::Vector3SubVector3), "vec3 - vec3", twoVector3Input, vector3Input},                // vec3 - vec3
    {n(EOperatorType::Vector3MulFloat), "float * vec3", floatVector3Input, vector3Input},               // float * vec3
    {n(EOperatorType::NormalizeVector3), "normalize vec3", vector3Input, vector3Input}, // normalize vec3
    {n(EOperatorType::Vector3Length), "length(vec3)", vector3Input, floatInput,
     "l = sqrt(x*x + y*y + z*z)"}, // lengthvec3
    {n(EOperatorType::ShowVector3), "show vec3", vector3Input, matrixInput,
     "Create the matrix rotating vector [1, 0, 0] to the input vector. Should "
     "be used for display of rotated "
     "vectors."},                                                                                          // show vec3
    {n(EOperatorType::MixVector3), "mix vec3", twoVector3FloatInput, vector3Input, NO_TAG, mixInputNames}, // mix vec3
    {n(EOperatorType::ConjQuat), "quat conjugate", quatInput, quatInput},                  // quat conjugate
    {n(EOperatorType::FloatVecToQuat), "quat(float, vec3)", floatVector3Input, quatInput}, // quatfloat, vec3
    {n(EOperatorType::AngleAxisToQuat), "quat(angle, axis)", twoFloatVector3Input, quatInput, NO_TAG,
     AngleAxisToQuatInputNames}, // quatangle, axis
    {n(EOperatorType::VecVecToQuat), "quat(vec3, vec3)", twoVector3Input, quatInput,
     "Input vector's are normalized."}, // quatvec3, vec3   //PF: floatInput
                                        // changed to quatInput
    {n(EOperatorType::QuatToFloatVec), "quat -> float, vec3", quatInput, floatVector3Input, NO_TAG, DEFAULT_NAMES,
     QuatToFloatVecInputNames}, // quat -> float, vec3
    {n(EOperatorType::QuatToAngleAxis), "quat -> angle, axis", quatInput, floatVector3Input, NO_TAG, DEFAULT_NAMES,
     AngleAxisInputNames},                                                        // quat -> angle, axis
    {n(EOperatorType::QuatToEuler), "quat -> euler", quatInput, threeFloatInput}, // quat -> euler
    {n(EOperatorType::EulerToQuat), "euler -> quat", threeFloatInput, quatInput, NO_TAG, {"x", "y", "z"}}, // euler
                                                                                                           // ->
                                                                                                           // quat
    {n(EOperatorType::QuatInverse), "inverse quat", quatInput, quatInput},                     // inverse quat
    {n(EOperatorType::QuatSlerp), "quat slerp", twoQuatFloatInput, quatInput},                 // quat slerp
    {n(EOperatorType::QuatLongWaySlerp), "quat long way slerp", twoQuatFloatInput, quatInput}, // quat long way slerp
    {n(EOperatorType::QuatLerp), "quat lerp", twoQuatFloatInput, quatInput},                   // quat lerp
    {n(EOperatorType::FloatMulQuat), "float * quat", floatQuatInput, quatInput},               // float * quat
    {n(EOperatorType::QuatMulQuat), "quat * quat", twoQuatInput, quatInput},                   // quat * quat
    {n(EOperatorType::QuatVecConjQuat), "qvq*", vector3QuatInput, vector3Input},               // qvq*

    {n(EOperatorType::QuatLength), "length(quat)", quatInput, floatInput,
     "l = sqrt(w*w + x*x + y*y + z*z)"}, // lengthquat
    {n(EOperatorType::ClampFloat), "clamp float", threeFloatInput, floatInput,
     "Clamp val to <min, max> interval.\n Default min = 0 and max = 1", ClampFloatInputNames},     // clamp float
    {n(EOperatorType::FloatMulFloat), "float * float", twoFloatInput, floatInput},                 // float * float
    {n(EOperatorType::FloatDivFloat), "float / float", twoFloatInput, floatInput},                 // float / float
    {n(EOperatorType::FloatAddFloat), "float + float", twoFloatInput, floatInput},                 // float + float
    {n(EOperatorType::FloatPowFloat), "float ^ float", twoFloatInput, floatInput},                 // float ^ float
    {n(EOperatorType::MixFloat), "mix float", threeFloatInput, floatInput, NO_TAG, mixInputNames}, // mix float
    {n(EOperatorType::FloatSinCos), "sin & cos", floatInput, twoFloatInput},                       // sin&cos(float)
    {n(EOperatorType::ASinACos), "asin & acos", floatInput, twoFloatInput},                        // asin&acos(float)
    {n(EOperatorType::Signum), "signum", floatInput, floatInput},                                  // signum
    {n(EOperatorType::MatrixToVectors), "mat -> vecs4", matrixInput, fourVectorInput},             // mat -> vecs
    {n(EOperatorType::Vectors3ToMatrix), "vecs3 -> mat", fourVector3Input, matrixInput, NO_TAG,
     Vectors3ToMatrixInputNames}, // vecs3 -> mat
    {n(EOperatorType::VectorsToMatrix), "vecs4 -> mat", fourVectorInput, matrixInput, NO_TAG,
     VectorsToMatrixInputNames}, // vecs4 -> mat
    {n(EOperatorType::MatrixToFloats), "mat -> floats", matrixInput, sixteenFloatInput, NO_TAG, DEFAULT_NAMES,
     matrixIndexNames()}, // mat -> floats
    {n(EOperatorType::FloatsToMatrix), "floats -> mat", sixteenFloatInput, matrixInput, NO_TAG,
     matrixIndexNames()}, // floats -> mat
    {n(EOperatorType::MatrixToTR), "mat -> TR", matrixInput, twoMatrixInput, NO_TAG, DEFAULT_NAMES, tr}, // mat -> TR
    {n(EOperatorType::TRToMatrix), "TR -> mat", twoMatrixInput, matrixInput, NO_TAG, tr},                // TR -> mat
    {n(EOperatorType::MatrixToQuat), "mat -> quat", matrixInput, quatInput},                             // mat -> quat
    {n(EOperatorType::QuatToMatrix), "quat -> mat", quatInput, matrixInput},                             // quat -> mat
    {n(EOperatorType::VectorToFloats), "vec4 -> floats", vectorInput, fourFloatInput, NO_TAG, DEFAULT_NAMES,
     xyzw},                                                                                          // vec4 -> float
    {n(EOperatorType::FloatsToVector), "floats -> vec4", fourFloatInput, vectorInput, NO_TAG, xyzw}, // floats -> vec4
    {n(EOperatorType::Vector3ToFloats), "vec3 -> floats", vector3Input, threeFloatInput, NO_TAG, DEFAULT_NAMES,
     xyz},                                                                                             // vec3 -> floats
    {n(EOperatorType::FloatsToVector3), "floats -> vec3", threeFloatInput, vector3Input, NO_TAG, xyz}, // floats -> vec3
    {n(EOperatorType::VectorToVector3), "vec4 -> vec3", vectorInput, vector3Input},                    // vec4 -> vec3
    //
    {n(EOperatorType::Vector3ToVector), "vec3 -> vec4", vector3FloatInput, vectorInput}, // vec3 -> vec4
    {n(EOperatorType::QuatToFloats), "quat -> floats", quatInput, fourFloatInput, NO_TAG, DEFAULT_NAMES,
     xyzw},                                                                                      // quat -> floats
    {n(EOperatorType::FloatsToQuat), "floats -> quat", fourFloatInput, quatInput, NO_TAG, xyzw}, // floats -> quat
    {n(EOperatorType::NormalizeQuat), "normalize quat", quatInput, quatInput},                   // normalize quat

    // Value nodes.
    {n(EOperatorType::FloatToFloat), "float", floatInput, floatInput, NO_TAG, DEFAULT_NAMES, DEFAULT_NAMES, true},
    {n(EOperatorType::Vector3ToVector3), "vec3", vector3Input, vector3Input, NO_TAG, DEFAULT_NAMES, DEFAULT_NAMES,
     true},
    {n(EOperatorType::Vector4ToVector4), "vec4", vectorInput, vectorInput, NO_TAG, DEFAULT_NAMES, DEFAULT_NAMES, true},
    {n(EOperatorType::MatrixToMatrix), "matrix", matrixInput, matrixInput, NO_TAG, DEFAULT_NAMES, DEFAULT_NAMES, true},
    {n(EOperatorType::QuatToQuat),
     "quat",
     {EValueType::Quat},
     {EValueType::Quat},
     NO_TAG,
     DEFAULT_NAMES,
     DEFAULT_NAMES,
     true},

    // Transform matrices constructors
    // PF: I have deleted the world constructor from all operators
    {n(EOperatorType::MakeTranslation), "translate", vector3Input, matrixInput},                     // translate
    {n(EOperatorType::MakeEulerX), "eulerAngleX", floatInput, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleX
    {n(EOperatorType::MakeEulerY), "eulerAngleY", floatInput, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleY
    {n(EOperatorType::MakeEulerZ), "eulerAngleZ", floatInput, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleZ
    {n(EOperatorType::MakeScale), "scale", vector3Input, matrixInput},                               // scale
    {n(EOperatorType::MakeAxisAngle), "rotate", floatVector3Input, matrixInput, NO_TAG, AngleAxisInputNames}, // rotate
    {n(EOperatorType::MakeOrtho), "ortho", sixFloatInput, matrixInput, NO_TAG, orthoFrustrumInputNames},      // ortho
    {n(EOperatorType::MakePerspective), "perspective", fourFloatInput, matrixInput, NO_TAG,
     PerspectiveInputNames}, // perspective
    {n(EOperatorType::MakeFrustum), "frustum", sixFloatInput, matrixInput, NO_TAG, orthoFrustrumInputNames}, // frustum
    {n(EOperatorType::MakeLookAt), "lookAt", threeVector3Input, matrixInput, NO_TAG, lookAtInputNames},      // lookAt

    Operation{n(EOperatorType::Screen),
              "screen",
              {EValueType::Screen},
              {EValueType::Screen, EValueType::Float},
              NO_TAG,
              {},
              {},
              false,
              true},
    {n(EOperatorType::Pulse), "pulse", {EValueType::Pulse}, {EValueType::Pulse}}};

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
	"Cycle", "cycle", cycleInputs, cycleOutputs,
    NO_TAG,
	{"from", "to", "step", "play", "pause", "stop", "prev", "next"},
	{"value", "play", "pause", "stop", "prev", "next", "end"}
};
// clang-format on

inline const Operation g_sequence = {
    "Sequence", "sequence", matrixMulAndMatrixInput, {EValueType::MatrixMul, EValueType::Matrix, EValueType::Matrix}};

inline const Operation g_cameraProperties = {
    "Camera", "camera", {}, {EValueType::Screen, EValueType::Matrix, EValueType::MatrixMul}};

inline static const Operation g_modelProperties = {
    "Model",
    "model",
    matrixMulInput,
    {},
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
        {{"angle", EValueType::Float}},
        true
    },
    {
        {n(ETransformType::EulerY), "eulerAngleY"},
        0b1010000010100000,
        {{"angle", EValueType::Float}},
        true
    },
    {
        {n(ETransformType::EulerZ), "eulerAngleZ"},
        0b1100110000000000,
        {{"angle", EValueType::Float}},
        true
    },
    {
        {n(ETransformType::Scale), "scale"},
     0b1000010000100000, // the diagonal
     {{"scale", EValueType::Vec3}}
    },
    {
        {n(ETransformType::AxisAngle), "axisAngle"},
        0b1110111011100000,
        {
            {"axis", EValueType::Vec3},
            {"angle", EValueType::Float}
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
}; // namespace Core

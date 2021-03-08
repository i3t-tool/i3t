/**
 * File with operations table.
 */
#pragma once

#include <sstream>

#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/transform.hpp"

#include "Core/Defs.h"

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

struct Operation
{
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
};

/// \todo rename to optype
enum class ENodeType
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
	Float,
	Vector3,
	Vector4,
	Matrix,
	Model
};

enum class ETransformType
{
	Translation,
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
static const std::vector<std::string> defaultIoNames = {
		"",       // PULSE		MN dodelat
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

static const std::vector<std::string> mixInputNames = {"from", "to", "t"};
static const std::vector<std::string> AngleAxisToQuatInputNames = {"angle", "angle / 2", "vec3"};
static const std::vector<std::string> QuatToFloatVecInputNames = {"w", "(x,y,z)"};
static const std::vector<std::string> AngleAxisInputNames = {"angle", "axis"};
static const std::vector<std::string> Vectors3ToMatrixInputNames = {"vec3 X", "vec3 Y", "vec3 Z", "vec3 T"};
static const std::vector<std::string> VectorsToMatrixInputNames = {"vec4 X", "vec4 Y", "vec4 Z", "vec4 T"};
static const std::vector<std::string> ClampFloatInputNames = {"val", "min", "max"};
static const std::vector<std::string> xyz = {"X", "Y", "Z"};
static const std::vector<std::string> xyzw = {"X", "Y", "Z", "W"};
static const std::vector<std::string> tr = {"T", "R"};
static const std::vector<std::string> eulerInputNames = {"angle"};
static const std::vector<std::string> orthoFrustrumInputNames = {"left", "right", "bottom", "top", "zNear", "zFar"};
static const std::vector<std::string> PerspectiveInputNamas = {"fovy", "aspect", "zNear", "zFar"};
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
		{"FloatToFloat", "float", 1, floatInput, 1, floatInput},
		{"Vector3ToVector3", "vec3", 1, vector3Input, 1, vector3Input},
		{"Vector4ToVector4", "vec4", 1, vectorInput, 1, vectorInput},
		{"MatrixToMatrix", "mat", 1, matrixInput, 1, matrixInput},

    {"Model", "model", 1, matrixInput, 0, {}}
};

namespace Core
{
static const Operation g_sequence = {"Sequence", "seq", 2, twoMatrixInput, 2, twoMatrixInput};

static const std::vector<Operation> g_transforms = {
		{"Translation", "translate", 1, vector3Input, 1, matrixInput},                                    // translate
		{"EulerX", "eulerAngleX", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames},                // eulerAngleX
		{"EulerY", "eulerAngleY", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames},                // eulerAngleY
		{"EulerZ", "eulerAngleZ", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames},                // eulerAngleZ
		{"Scale", "scale", 1, vector3Input, 1, matrixInput},                                              // scale
		{"AxisAngle", "rotate", 2, floatVector3Input, 1, matrixInput, NO_TAG, AngleAxisInputNames},       // rotate
		{"Quat", "quat", 2, quatInput, 1, matrixInput, NO_TAG, AngleAxisInputNames},                      // quat rotate
		{"Ortho", "ortho", 6, sixFloatInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames},            // ortho
		{"Perspective", "perspective", 4, fourFloatInput, 1, matrixInput, NO_TAG, PerspectiveInputNamas}, // perspective
		{"Frustum", "frustum", 6, sixFloatInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames},        // frustrum
		{"LookAt", "lookAt", 3, threeVector3Input, 1, matrixInput, NO_TAG, lookAtInputNames},             // lookAt
};

FORCE_INLINE const Operation* getTransformProps(ETransformType type)
{
	return &g_transforms[static_cast<unsigned>(type)];
}
}; // namespace Core

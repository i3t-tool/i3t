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

#define NO_TAG				""
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

static const ValidDataMaps defaultDataMaps = {&Transform::g_AllLocked, &Transform::g_Free};
} // namespace Core

using PinGroup = std::vector<EValueType>;

struct Operation
{
	using Pins = std::vector<EValueType>;

	const std::string							 keyWord;
	const std::string							 defaultLabel;
	const int											 numberOfInputs;
	const std::vector<EValueType>	 inputTypes;
	const int											 numberOfOutputs;
	const std::vector<EValueType>	 outputTypes;
	const std::string							 defaultTagText		 = NO_TAG;
	const std::vector<std::string> defaultInputNames = DEFAULT_NAMES; // if the names are not the names of the OpValueType
	const std::vector<std::string> defaultOutputNames =
			DEFAULT_NAMES; // if the names are not the names of the OpValueType
	const std::vector<const Core::Transform::DataMap*> validDatamaps = {&Core::Transform::g_AllLocked};

	/*
	Operation(const Operation&) = delete;
	Operation(const Operation&&) = delete;
	Operation& operator=(const Operation&) = delete;
	Operation& operator=(const Operation&&) = delete;
	 */

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
						const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
						const std::vector<EValueType>& outputTypes) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
			numberOfOutputs(numberOfOutputs), outputTypes(outputTypes)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
						const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
						const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
						const std::vector<std::string>&											defaultInputNames,
						const std::vector<const Core::Transform::DataMap*>& validDatamaps) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
			numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
			defaultInputNames(defaultInputNames), validDatamaps(validDatamaps)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
						const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
						const std::vector<EValueType>&											outputTypes,
						const std::vector<const Core::Transform::DataMap*>& validDatamaps) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
			numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), validDatamaps(validDatamaps)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
						const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
						const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
						const std::vector<std::string>& defaultInputNames, const std::vector<std::string>& defaultOutputNames) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
			numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
			defaultInputNames(defaultInputNames), defaultOutputNames(defaultOutputNames)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
						const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
						const std::vector<EValueType>& outputTypes, const std::string& defaultTagText,
						const std::vector<std::string>& defaultInputNames) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
			numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText),
			defaultInputNames(defaultInputNames)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel, const int numberOfInputs,
						const std::vector<EValueType>& inputTypes, const int numberOfOutputs,
						const std::vector<EValueType>& outputTypes, const std::string& defaultTagText) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), numberOfInputs(numberOfInputs), inputTypes(inputTypes),
			numberOfOutputs(numberOfOutputs), outputTypes(outputTypes), defaultTagText(defaultTagText)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel,
						const std::vector<const Core::Transform::DataMap*>& validDatamaps) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), validDatamaps(validDatamaps), numberOfInputs(0), numberOfOutputs(0)
	{}

	Operation(const std::string& keyWord, const std::string& defaultLabel,
						const std::vector<std::string>&											defaultInputNames,
						const std::vector<const Core::Transform::DataMap*>& validDatamaps) :
			keyWord(keyWord),
			defaultLabel(defaultLabel), defaultInputNames(defaultInputNames), validDatamaps(validDatamaps), numberOfInputs(0),
			numberOfOutputs(0)
	{}
};

enum class ENodeType
{
	Inversion,
	Transpose,
	Determinant,
	MatrixMulMatrix,
	MatrixAddMatrix,					 // done SS
	MatrixMulVector,					 // done SS
	VectorMulMatrix,					 // done SS
	MatrixMulFloat,						 // done SS
	VectorDotVector,					 // done SS
	VectorAddVector,					 // done SS
	VectorSubVector,					 // done SS
	VectorMulFloat,						 // done SS
	VectorPerspectiveDivision, // done SS
	NormalizeVector,					 // done JH
	MixVector,								 // done SS
	Vector3CrossVector3,			 // done SS
	Vector3DotVector3,				 // done SS
	Vector3AddVector3,				 // done SS
	Vector3SubVector3,				 // done SS
	Vector3MulFloat,					 // done SS
	NormalizeVector3,					 // done SS
	Vector3Length,						 // done SS
	ShowVector3,							 // done SS
	MixVector3,								 // done SS
	ConjQuat,									 // done SS
	FloatVecToQuat,						 // done SS
	AngleAxisToQuat,					 // done SS
	VecVecToQuat,							 // done SS
	QuatToFloatVec,						 // done SS
	QuatToAngleAxis,					 // done SS
	QuatToEuler,							 // done SS
	EulerToQuat,							 // done SS
	QuatInverse,							 // done SS
	QuatSlerp,								 // done SS
	QuatLongWaySlerp,					 // done SS
	QuatLerp,									 // done SS
	FloatMulQuat,							 // done SS
	QuatMulQuat,							 // done SS
	QuatVecConjQuat,					 // done SS
	QuatLength,								 // done SS
	ClampFloat,								 // done SS
	FloatMulFloat,						 // done SS
	FloatDivFloat,						 // done SS
	FloatAddFloat,						 // done SS
	FloatPowFloat,						 // done SS
	MixFloat,									 // done SS
	FloatSinCos,							 // done SS
	ASinACos,									 // done SS
	Signum,										 // done SS
	MatrixToVectors,					 // done SS
	Vectors3ToMatrix,					 // done SS
	VectorsToMatrix,					 // done SS
	MatrixToFloats,						 // done SS
	FloatsToMatrix,						 // done SS
	MatrixToTR,								 // done SS
	TRToMatrix,								 // done SS
	MatrixToQuat,							 // done SS
	QuatToMatrix,							 // done SS
	VectorToFloats,						 // done SS
	FloatsToVector,						 // done SS
	Vector3ToFloats,					 // done SS
	FloatsToVector3,					 // done SS
	VectorToVector3,					 // done SS
	Vector3ToVector,					 // done SS
	QuatToFloats,							 // done SS
	FloatsToQuat,							 // done SS
	NormalizeQuat,						 // done SS
	FloatToFloat,							 // done SS
	Vector3ToVector3,					 // done JH
	Vector4ToVector4,					 // done JH
	MatrixToMatrix,						 // done JH
	QuatToQuat,								 // done SS
	Model,

	// Transform matrices "constructors"
	MakeTranslation, // done SS
	MakeEulerX,			 // done SS
	MakeEulerY,			 // done SS
	MakeEulerZ,			 // done SS
	MakeScale,			 // done SS
	MakeAxisAngle,	 // done SS
	MakeOrtho,			 // done SS
	MakePerspective, // done SS
	MakeFrustum,		 // done SS
	MakeLookAt,			 // done SS

	Screen, // done SS
	Pulse		// done SS

	// SS trackball (trackcube) is done, waiting for Core
};


enum class ETransformType
{
	// This is for a sequence.
	Free = 0,		 // done SS
	Translation, // done JH
	EulerX,			 // done SS
	EulerY,			 // done SS
	EulerZ,			 // done SS
	Scale,			 // done JH
	AxisAngle,	 // done SS
	Quat,				 // done SS
	Ortho,			 // done SS
	Perspective, // done SS
	Frustum,		 // done SS
	LookAt,			 // done SS
};

// pro kazdy OpValueType (NodeData.h) je jeden string
static const std::array<const char*, 8> defaultIoNames = {
		"pulse",	// PULSE		MN dodelat
		"float",	// Float
		"vec3",		// Vec3
		"vec4",		// Vec4
		"matrix", // Matrix
		"quat",		// Quat
		"",				// Matrix_MULL	MN dodelat
		""				// SCREEN		MN dodelat
};

static const std::vector<EValueType> matrixInput	= {EValueType::Matrix};
static const std::vector<EValueType> vectorInput	= {EValueType::Vec4};
static const std::vector<EValueType> vector3Input = {EValueType::Vec3};
static const std::vector<EValueType> floatInput		= {EValueType::Float};
static const std::vector<EValueType> quatInput		= {EValueType::Quat};

static const std::vector<EValueType> twoMatrixInput	 = {EValueType::Matrix, EValueType::Matrix};
static const std::vector<EValueType> twoVectorInput	 = {EValueType::Vec4, EValueType::Vec4};
static const std::vector<EValueType> twoVector3Input = {EValueType::Vec3, EValueType::Vec3};
static const std::vector<EValueType> twoQuatInput		 = {EValueType::Quat, EValueType::Quat};
static const std::vector<EValueType> twoFloatInput	 = {EValueType::Float, EValueType::Float};

static const std::vector<EValueType> threeFloatInput	 = {EValueType::Float, EValueType::Float, EValueType::Float};
static const std::vector<EValueType> threeVector3Input = {EValueType::Vec3, EValueType::Vec3, EValueType::Vec3};

static const std::vector<EValueType> fourVectorInput	= {EValueType::Vec4, EValueType::Vec4, EValueType::Vec4,
																												 EValueType::Vec4};
static const std::vector<EValueType> fourVector3Input = {EValueType::Vec3, EValueType::Vec3, EValueType::Vec3,
																												 EValueType::Vec3};
static const std::vector<EValueType> fourFloatInput		= {EValueType::Float, EValueType::Float, EValueType::Float,
																											 EValueType::Float};

static const std::vector<EValueType> sixFloatInput = {EValueType::Float, EValueType::Float, EValueType::Float,
																											EValueType::Float, EValueType::Float, EValueType::Float};

static const std::vector<EValueType> sixteenFloatInput = {
		EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float,
		EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float,
		EValueType::Float, EValueType::Float, EValueType::Float, EValueType::Float};

static const std::vector<EValueType> twoVectorFloatInput	= {EValueType::Vec4, EValueType::Vec4, EValueType::Float};
static const std::vector<EValueType> twoVector3FloatInput = {EValueType::Vec3, EValueType::Vec3, EValueType::Float};
static const std::vector<EValueType> twoFloatVector3Input = {EValueType::Float, EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> twoQuatFloatInput		= {EValueType::Quat, EValueType::Quat, EValueType::Float};

static const std::vector<EValueType> matrixVectorInput = {EValueType::Matrix, EValueType::Vec4};
static const std::vector<EValueType> vectorMatrixInput = {EValueType::Vec4, EValueType::Matrix};
static const std::vector<EValueType> floatMatrixInput	 = {EValueType::Float, EValueType::Matrix};
static const std::vector<EValueType> floatVectorInput	 = {EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> floatVector3Input = {EValueType::Float, EValueType::Vec3};
static const std::vector<EValueType> floatQuatInput		 = {EValueType::Float, EValueType::Quat};
static const std::vector<EValueType> quatVector3Input	 = {EValueType::Quat, EValueType::Vec3};

static const std::vector<EValueType> matrixMulAndMatrixInput = {EValueType::MatrixMul, EValueType::Matrix};
static const std::vector<EValueType> matrixMulInput					 = {EValueType::MatrixMul};

static const std::vector<std::string> mixInputNames							 = {"from", "to", "t"};
static const std::vector<std::string> AngleAxisToQuatInputNames	 = {"angle", "angle / 2", "vec3"};
static const std::vector<std::string> QuatToFloatVecInputNames	 = {"w", "(x,y,z)"};
static const std::vector<std::string> AngleAxisInputNames				 = {"angle", "axis"};
static const std::vector<std::string> Vectors3ToMatrixInputNames = {"vec3 X", "vec3 Y", "vec3 Z", "vec3 T"};
static const std::vector<std::string> VectorsToMatrixInputNames	 = {"vec4 X", "vec4 Y", "vec4 Z", "vec4 T"};
static const std::vector<std::string> ClampFloatInputNames			 = {"val", "min", "max"};
static const std::vector<std::string> xyz												 = {"x", "y", "z"};
static const std::vector<std::string> xyzw											 = {"x", "y", "z", "w"};
static const std::vector<std::string> tr												 = {"T", "R"};
static const std::vector<std::string> eulerInputNames						 = {"angle"};
static const std::vector<std::string> orthoFrustrumInputNames		 = {"left", "right", "bottom", "top", "zNear", "zFar"};
static const std::vector<std::string> PerspectiveInputNames			 = {"fovy", "aspect", "zNear", "zFar"};
static const std::vector<std::string> lookAtInputNames					 = {"eye", "center", "up"};

/**
 * \brief Table box with configuration parameters
 * \nevim VectorMulMatrix -  vec4 * mat	MN jaky je duvod teto operace a nema byt vysledek matice???
 */
static const std::vector<Operation> operations = {
		{n(ENodeType::Inversion), "inversion", 1, matrixInput, 1, matrixInput},							 // inversion
		{n(ENodeType::Transpose), "transpose", 1, matrixInput, 1, matrixInput},							 // transpose
		{n(ENodeType::Determinant), "determinant", 1, matrixInput, 1, floatInput},					 // determinant
		{n(ENodeType::MatrixMulMatrix), "mat * mat", 2, twoMatrixInput, 1, matrixInput},		 // mat * mat
		{n(ENodeType::MatrixAddMatrix), "mat + mat", 2, twoMatrixInput, 1, matrixInput},		 // mat + mat
		{n(ENodeType::MatrixMulVector), "mat * vec4", 2, matrixVectorInput, 1, vectorInput}, // mat * vec4
		{n(ENodeType::VectorMulMatrix), "vec4 * mat", 2, vectorMatrixInput, 1,
		 vectorInput}, // vec4 * mat	MN jaky je duvod teto operace a nema byt vysledek matice???
		{n(ENodeType::MatrixMulFloat), "float * mat", 2, floatMatrixInput, 1, matrixInput},										// float * mat
		{n(ENodeType::VectorDotVector), "vec4 . vec4", 2, twoVectorInput, 1, floatInput},											// vec4 . vec4
		{n(ENodeType::VectorAddVector), "vec4 + vec4", 2, twoVectorInput, 1, vectorInput},										// vec4 + vec4
		{n(ENodeType::VectorSubVector), "vec4 - vec4", 2, twoVectorInput, 1, vectorInput},										// vec4 - vec4
		{n(ENodeType::VectorMulFloat), "float * vec4", 2, floatVectorInput, 1, vectorInput},									// float * vec4
		{n(ENodeType::VectorPerspectiveDivision), "perspective division", 1, vectorInput, 1, vectorInput},		// perspective division
		{n(ENodeType::NormalizeVector), "normalize vec4", 1, vectorInput, 1, vectorInput},										// normalize vec4
		{n(ENodeType::MixVector), "mix vec4", 3, twoVectorFloatInput, 1, vectorInput, NO_TAG, mixInputNames}, // mix vec4
		{n(ENodeType::Vector3CrossVector3), "vec3 x vec3", 2, twoVector3Input, 1, vector3Input},							// vec3 x vec3
		{n(ENodeType::Vector3DotVector3), "vec3 . vec3", 2, twoVector3Input, 1, floatInput},									// vec3 . vec3
		{n(ENodeType::Vector3AddVector3), "vec3 + vec3", 2, twoVector3Input, 1, vector3Input},								// vec3 + vec3
		{n(ENodeType::Vector3SubVector3), "vec3 - vec3", 2, twoVector3Input, 1, vector3Input},								// vec3 - vec3
		{n(ENodeType::Vector3MulFloat), "float * vec3", 2, floatVectorInput, 1, vector3Input},								// float * vec3
		{n(ENodeType::NormalizeVector3), "normalize vec3", 1, vector3Input, 1, vector3Input},									// normalize vec3
		{n(ENodeType::Vector3Length), "lengthvec3", 1, vector3Input, 1, floatInput, "l = sqrt(x*x + y*y + z*z)"}, // lengthvec3
		{n(ENodeType::ShowVector3), "show vec3", 1, vector3Input, 1, matrixInput,
		 "Create the matrix transforming vector [1, 0, 0] to the input vector. Should be used with bind objects / basic "
		 "/ vectors."},																																							 // show vec3
		{n(ENodeType::MixVector3), "mix vec3", 3, twoVector3FloatInput, 1, vector3Input, NO_TAG, mixInputNames}, // mix vec3
		{n(ENodeType::ConjQuat), "quat conjugate", 1, quatInput, 1, quatInput},																	 // quat conjugate
		{n(ENodeType::FloatVecToQuat), "quatfloat, vec3", 2, floatVector3Input, 1, quatInput},									 // quatfloat, vec3
		{n(ENodeType::AngleAxisToQuat), "quatangle, axis", 3, twoFloatVector3Input, 1, quatInput, NO_TAG,
		 AngleAxisToQuatInputNames}, // quatangle, axis
		{n(ENodeType::VecVecToQuat), "quatvec3, vec3", 2, twoVector3Input, 1, floatInput,
		 "Input vector's are normalized."}, // quatvec3, vec3
		{n(ENodeType::QuatToFloatVec), "quat -> float, vec3", 1, quatInput, 2, floatVector3Input, NO_TAG, DEFAULT_NAMES,
		 QuatToFloatVecInputNames}, // quat -> float, vec3
		{n(ENodeType::QuatToAngleAxis), "quat -> angle, axis", 1, quatInput, 2, floatVector3Input, NO_TAG, DEFAULT_NAMES,
		 AngleAxisInputNames},																																				// quat -> angle, axis
		{n(ENodeType::QuatToEuler), "quat -> euler", 1, quatInput, 3, threeFloatInput},														// quat -> euler
		{n(ENodeType::EulerToQuat), "euler -> quat", 3, threeFloatInput, 1, quatInput, NO_TAG, {"x", "y", "z"}},	// euler -> quat
		{n(ENodeType::QuatInverse), "inverse quat", 1, quatInput, 1, quatInput},																	// inverse quat
		{n(ENodeType::QuatSlerp), "quat slerp", 3, twoQuatFloatInput, 1, quatInput},															// quat slerp
		{n(ENodeType::QuatLongWaySlerp), "quat long way slerp", 3, twoQuatFloatInput, 1, quatInput},							// quat long way slerp
		{n(ENodeType::QuatLerp), "quat lerp", 3, twoQuatFloatInput, 1, quatInput},																// quat lerp
		{n(ENodeType::FloatMulQuat), "float * quat", 2, floatQuatInput, 1, quatInput},														// float * quat
		{n(ENodeType::QuatMulQuat), "quat * quat", 2, twoQuatInput, 1, quatInput},																// quat * quat
		{n(ENodeType::QuatVecConjQuat), "qvq*", 2, quatVector3Input, 1, vector3Input},														// qvq*
		{n(ENodeType::QuatLength), "lengthquat", 1, quatInput, 1, floatInput, "l = sqrt(w*w + x*x + y*y + z*z)"}, // lengthquat
		{n(ENodeType::ClampFloat), "clamp float", 3, threeFloatInput, 1, floatInput,
		 "Clamp val to <min, max> interval.\n Default min = 0 and max = 1", ClampFloatInputNames}, // clamp float
		{n(ENodeType::FloatMulFloat), "float * float", 2, twoFloatInput, 1, floatInput},											 // float * float
		{n(ENodeType::FloatDivFloat), "float / float", 2, twoFloatInput, 1, floatInput},											 // float / float
		{n(ENodeType::FloatAddFloat), "float + float", 2, twoFloatInput, 1, floatInput},											 // float + float
		{n(ENodeType::FloatPowFloat), "float ^ float", 2, twoFloatInput, 1, floatInput},											 // float ^ float
		{n(ENodeType::MixFloat), "mix float", 3, threeFloatInput, 1, floatInput, NO_TAG, mixInputNames},			 // mix float
		{n(ENodeType::FloatSinCos), "sin&cosfloat", 1, floatInput, 2, twoFloatInput},													 // sin&cosfloat
		{n(ENodeType::ASinACos), "asin&acosfloat", 1, floatInput, 2, twoFloatInput},													 // asin&acosfloat
		{n(ENodeType::Signum), "signum", 1, floatInput, 1, floatInput},																				 // signum
		{n(ENodeType::MatrixToVectors), "mat -> vecs", 1, matrixInput, 4, fourVectorInput},										 // mat -> vecs
		{n(ENodeType::Vectors3ToMatrix), "vecs3 -> mat", 4, fourVector3Input, 1, matrixInput, NO_TAG,
		 Vectors3ToMatrixInputNames}, // vecs3 -> mat
		{n(ENodeType::VectorsToMatrix), "vecs4 -> mat", 4, fourVectorInput, 1, matrixInput, NO_TAG,
		 VectorsToMatrixInputNames}, // vecs4 -> mat
		{n(ENodeType::MatrixToFloats), "mat -> floats", 1, matrixInput, 16, sixteenFloatInput, NO_TAG, DEFAULT_NAMES,
		 matrixIndexNames()}, // mat -> floats
		{n(ENodeType::FloatsToMatrix), "floats -> mat", 16, sixteenFloatInput, 1, matrixInput, NO_TAG,
		 matrixIndexNames()},																																			 // floats -> mat
		{n(ENodeType::MatrixToTR), "mat -> TR", 1, matrixInput, 2, twoMatrixInput, NO_TAG, DEFAULT_NAMES, tr}, // mat -> TR
		{n(ENodeType::TRToMatrix), "TR -> mat", 2, twoMatrixInput, 1, matrixInput, NO_TAG, tr},								 // TR -> mat
		{n(ENodeType::MatrixToQuat), "mat -> quat", 1, matrixInput, 1, quatInput},														 // mat -> quat
		{n(ENodeType::QuatToMatrix), "quat -> mat", 1, quatInput, 1, matrixInput},														 // quat -> mat
		{n(ENodeType::VectorToFloats), "vec4 -> floats", 1, vectorInput, 4, fourFloatInput, NO_TAG, DEFAULT_NAMES,
		 xyzw},																																								 // vec4 -> float
		{n(ENodeType::FloatsToVector), "floats -> vec4", 4, fourFloatInput, 1, vectorInput, NO_TAG, xyzw}, // floats -> vec4
		{n(ENodeType::Vector3ToFloats), "vec3 -> floats", 1, vector3Input, 3, threeFloatInput, NO_TAG, DEFAULT_NAMES,
		 xyz},																																														// vec3 -> floats
		{n(ENodeType::FloatsToVector3), "floats -> vec3", 3, threeFloatInput, 1, vector3Input, NO_TAG, xyz},					// floats -> vec3
		{n(ENodeType::VectorToVector3), "vec4 -> vec3", 1, vectorInput, 1, vector3Input},															// vec4 -> vec3
		{n(ENodeType::Vector3ToVector), "vec3 -> vec4", 1, vector3Input, 1, vectorInput},															// vec3 -> vec4
		{n(ENodeType::QuatToFloats), "quat -> floats", 1, quatInput, 4, fourFloatInput, NO_TAG, DEFAULT_NAMES, xyzw}, // quat -> floats
		{n(ENodeType::FloatsToQuat), "floats -> quat", 4, fourFloatInput, 1, quatInput, NO_TAG, xyzw},								// floats -> quat
		{n(ENodeType::NormalizeQuat), "normalize quat", 1, quatInput, 1, quatInput},																	// normalize quat

		// Value nodes.
		{n(ENodeType::FloatToFloat), "float", 1, floatInput, 1, floatInput, Core::defaultDataMaps},
		{n(ENodeType::Vector3ToVector3), "vec3", 1, vector3Input, 1, vector3Input, Core::defaultDataMaps},
		{n(ENodeType::Vector4ToVector4), "vec4", 1, vectorInput, 1, vectorInput, Core::defaultDataMaps},
		{n(ENodeType::MatrixToMatrix), "mat", 1, matrixInput, 1, matrixInput, Core::defaultDataMaps},
		{n(ENodeType::QuatToQuat), "quat", 1, {EValueType::Quat}, 1, {EValueType::Quat}, Core::defaultDataMaps},

		{n(ENodeType::Model), "model", 1, matrixMulInput, 0, {}},

		// Transform matrices constructors
		{n(ENodeType::MakeTranslation), "translate constructor", 1, vector3Input, 1, matrixInput},										 // translate
		{n(ENodeType::MakeEulerX), "eulerAngleX constructor", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleX
		{n(ENodeType::MakeEulerY), "eulerAngleY constructor", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleY
		{n(ENodeType::MakeEulerZ), "eulerAngleZ constructor", 1, floatInput, 1, matrixInput, NO_TAG, eulerInputNames}, // eulerAngleZ
		{n(ENodeType::MakeScale), "scale constructor", 1, vector3Input, 1, matrixInput},															 // scale
		{n(ENodeType::MakeAxisAngle), "rotate constructor", 2, floatVector3Input, 1, matrixInput, NO_TAG,
		 AngleAxisInputNames},																																								 // rotate
		{n(ENodeType::MakeOrtho), "ortho constructor", 6, sixFloatInput, 1, matrixInput, NO_TAG, orthoFrustrumInputNames}, // ortho
		{n(ENodeType::MakePerspective), "perspective constructor", 4, fourFloatInput, 1, matrixInput, NO_TAG,
		 PerspectiveInputNames}, // perspective
		{n(ENodeType::MakeFrustum), "frustum constructor", 6, sixFloatInput, 1, matrixInput, NO_TAG,
		 orthoFrustrumInputNames},																																						// frustrum
		{n(ENodeType::MakeLookAt), "lookAt constructor", 3, threeVector3Input, 1, matrixInput, NO_TAG, lookAtInputNames}, // lookAt

		{n(ENodeType::Screen), "screen", 1, {EValueType::Screen}, 1, {EValueType::Float}},
		{n(ENodeType::Pulse), "pulse", 0, {}, 1, {EValueType::Pulse}}};

namespace Core
{
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

static const Operation g_CycleProperties = {"Cycle",
																						"cycle",
																						8,
																						cycleInputs,
																						7,
																						cycleOutputs,
																						NO_TAG,
																						{"from", "to", "step", "play", "pause", "stop", "prev", "next"},
																						{"value", "play", "pause", "stop", "prev", "next", "end"}};

inline const Operation g_sequence = {
		"Sequence", "seq", 2, matrixMulAndMatrixInput, 3, {EValueType::MatrixMul, EValueType::Matrix, EValueType::Matrix}};

static const Operation g_cameraProperties = {
		"Camera", "camera", 0, {}, 3, {EValueType::Screen, EValueType::Matrix, EValueType::MatrixMul}};

static const std::vector<Operation> g_transforms = {
		{n(ETransformType::Free), "free", defaultDataMaps},
		{n(ETransformType::Translation), "translate", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Translate}},
		{n(ETransformType::EulerX), "eulerAngleX", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_EulerX}},
		{n(ETransformType::EulerY), "eulerAngleY", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_EulerY}},
		{n(ETransformType::EulerZ), "eulerAngleZ", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_EulerZ}},
		{n(ETransformType::Scale), "scale", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Scale}},
		{n(ETransformType::AxisAngle), "rotate", defaultDataMaps},
		{n(ETransformType::Quat), "quat", defaultDataMaps},
		{n(ETransformType::Ortho), "ortho", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Ortho}},
		{n(ETransformType::Perspective), "perspective", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Perspective}},
		{n(ETransformType::Frustum), "frustum", {&Transform::g_AllLocked, &Transform::g_Free, &Transform::g_Frustum}},
		{n(ETransformType::LookAt), "lookAt", defaultDataMaps},
};

FORCE_INLINE const Operation* getOperationProps(ENodeType type) { return &operations[static_cast<size_t>(type)]; }

FORCE_INLINE const Operation* getTransformProps(ETransformType type)
{
	return &g_transforms[static_cast<unsigned>(type)];
}
}; // namespace Core

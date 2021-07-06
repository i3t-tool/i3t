
//transformations{
#include "WorkspaceAxisAngle.h"
#include "WorkspaceEulerX.h"
#include "WorkspaceEulerY.h"
#include "WorkspaceEulerZ.h"
#include "WorkspaceFrustum.h"
#include "WorkspaceLookAt.h"
#include "WorkspaceMatrixScale.h"
#include "WorkspaceMatrixTranslation.h"
#include "WorkspaceOrtho.h"
#include "WorkspacePerspective.h"
#include "WorkspaceSequence.h"
#include "WorkspaceTransformationFree.h"
#include "WorkspaceQuatRot.h"
//} transformations end

//operators {

//	transformations{
#include "WorkspaceMakeAxisAngle.h"
#include "WorkspaceMakeEulerX.h"
#include "WorkspaceMakeEulerY.h"
#include "WorkspaceMakeEulerZ.h"
#include "WorkspaceMakeFrustum.h"
#include "WorkspaceMakeLookAt.h"
#include "WorkspaceMakeOrtho.h"
#include "WorkspaceMakePerspective.h"
#include "WorkspaceMakeScale.h"
#include "WorkspaceMakeTranslation.h"
//	} transformations end

//	matrix{
#include "WorkspaceMatrixFree.h"
#include "WorkspaceMatrixInversion.h"
#include "WorkspaceMatrixMulMatrix.h"
#include "WorkspaceMatrixTranspose.h"
#include "WorkspaceDeterminant.h"
#include "WorkspaceMatrixAddMatrix.h"
#include "WorkspaceMatrixMulFloat.h"
#include "WorkspaceMatrixMulVector.h"
#include "WorkspaceVectorMulMatrix.h"
#include "WorkspaceTrackball.h"
//	} matrix end

//	vec4{
#include "WorkspaceVectorFree.h"
#include "WorkspaceNormalizeVector.h"
#include "WorkspaceVectorDotVector.h"
#include "WorkspaceVectorAddVector.h"
#include "WorkspaceVectorSubVector.h"
#include "WorkspaceVectorMulFloat.h"
#include "WorkspaceVectorPerspectiveDivision.h"
#include "WorkspaceMixVector.h"
//	} vec4 end

//	vec3{
#include "WorkspaceVector3Free.h"
#include "WorkspaceNormalizeVector3.h"
#include "WorkspaceVector3CrossVector3.h"
#include "WorkspaceVector3DotVector3.h"
#include "WorkspaceVector3AddVector3.h"
#include "WorkspaceVector3SubVector3.h"
#include "WorkspaceVector3MulFloat.h"
#include "WorkspaceVector3Length.h"
#include "WorkspaceShowVector3.h"
#include "WorkspaceMixVector3.h"
//	} vec4 end

//	float{
#include "WorkspaceFloatFree.h"
#include "WorkspaceClampFloat.h"
#include "WorkspaceFloatMulFloat.h"
#include "WorkspaceFloatDivFloat.h"
#include "WorkspaceFloatAddFloat.h"
#include "WorkspaceFloatPowFloat.h"
#include "WorkspaceMixFloat.h"
#include "WorkspaceSignum.h"
#include "WorkspaceFloatSinCos.h"
#include "WorkspaceASinACos.h"
#include "WorkspaceCycle.h"
//	} float end

//	quat{
#include "WorkspaceQuatVecConjQuat.h"
#include "WorkspaceQuatLength.h"
#include "WorkspaceQuatFree.h"
#include "WorkspaceFloatVecToQuat.h"
#include "WorkspaceAngleAxisToQuat.h"
#include "WorkspaceVecVecToQuat.h"
#include "WorkspaceFloatMulQuat.h"
#include "WorkspaceQuatMulQuat.h"
#include "WorkspaceEulerToQuat.h"
#include "WorkspaceQuatSlerp.h"
#include "WorkspaceQuatLerp.h"
#include "WorkspaceQuatLongWaySlerp.h"
#include "WorkspaceConjQuat.h"
#include "WorkspaceQuatInverse.h"
#include "WorkspaceNormalizeQuat.h"
#include "WorkspaceQuatToFloatVec.h"
#include "WorkspaceQuatToAngleAxis.h"
//	} quat end

//  conversion{
#include "WorkspaceFloatsToMatrix.h"
#include "WorkspaceFloatsToVector.h"
#include "WorkspaceFloatsToVector3.h"
#include "WorkspaceQuatToMatrix.h"
#include "WorkspaceTRToMatrix.h"
#include "WorkspaceVector3ToVector.h"
#include "WorkspaceVectors3ToMatrix.h"
#include "WorkspaceVectorsToMatrix.h"
#include "WorkspaceVectorToVector3.h"
#include "WorkspaceQuatToEuler.h"
#include "WorkspaceVectorToFloats.h"
#include "WorkspaceVector3ToFloats.h"
#include "WorkspaceQuatToFloats.h"
#include "WorkspaceMatrixToFloats.h"
#include "WorkspaceMatrixToTR.h"
#include "WorkspaceMatrixToVectors.h"
#include "WorkspaceMatrixToQuat.h"
#include "WorkspaceFloatsToQuat.h"
//  }
//}operators end

#include "WorkspaceModel.h"
#include "WorkspacePulse.h"
#include "WorkspaceScreen.h"

#pragma once

#include "WorkspaceTransformation.h"

class WorkspaceTransformationFree : public WorkspaceTransformation { public: WorkspaceTransformationFree(DIWNE::Diwne& diwne); };
class WorkspaceTransformationAxisAngle : public WorkspaceTransformation { public: WorkspaceTransformationAxisAngle(DIWNE::Diwne& diwne); };
class WorkspaceTransformationLookAt : public WorkspaceTransformation { public: WorkspaceTransformationLookAt(DIWNE::Diwne& diwne); };

class WorkspaceTransformationTranslation : public WorkspaceTransformation { public: WorkspaceTransformationTranslation(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationEulerX : public WorkspaceTransformation { public: WorkspaceTransformationEulerX(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationEulerY : public WorkspaceTransformation { public: WorkspaceTransformationEulerY(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationEulerZ : public WorkspaceTransformation { public: WorkspaceTransformationEulerZ(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationScale : public WorkspaceTransformation { public: WorkspaceTransformationScale(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationOrthoProjection : public WorkspaceTransformation { public: WorkspaceTransformationOrthoProjection(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationPerspective : public WorkspaceTransformation { public: WorkspaceTransformationPerspective(DIWNE::Diwne& diwne); bool drawDataSetValues(); };
class WorkspaceTransformationFrustum : public WorkspaceTransformation { public: WorkspaceTransformationFrustum(DIWNE::Diwne& diwne); bool drawDataSetValues(); };

class WorkspaceTransformationQuaternion : public WorkspaceTransformation { public: WorkspaceTransformationQuaternion(DIWNE::Diwne& diwne); bool drawDataFull(); int maxLenghtOfData(); };



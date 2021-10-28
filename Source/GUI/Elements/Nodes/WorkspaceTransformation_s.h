#pragma once
#include "WorkspaceTransformation.h"

class WorkspaceTransformationFree : public WorkspaceTransformation { public: WorkspaceTransformationFree(); };
class WorkspaceTransformationAxisAngle : public WorkspaceTransformation { public: WorkspaceTransformationAxisAngle(); };
class WorkspaceTransformationLookAt : public WorkspaceTransformation { public: WorkspaceTransformationLookAt(); };

class WorkspaceTransformationTranslation : public WorkspaceTransformation { public: WorkspaceTransformationTranslation(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationEulerX : public WorkspaceTransformation { public: WorkspaceTransformationEulerX(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationEulerY : public WorkspaceTransformation { public: WorkspaceTransformationEulerY(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationEulerZ : public WorkspaceTransformation { public: WorkspaceTransformationEulerZ(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationScale : public WorkspaceTransformation { public: WorkspaceTransformationScale(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationOrthoProjection : public WorkspaceTransformation { public: WorkspaceTransformationOrthoProjection(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationPerspective : public WorkspaceTransformation { public: WorkspaceTransformationPerspective(); bool drawDataSetValues(DIWNE::Diwne &diwne); };
class WorkspaceTransformationFrustum : public WorkspaceTransformation { public: WorkspaceTransformationFrustum(); bool drawDataSetValues(DIWNE::Diwne &diwne); };

class WorkspaceTransformationQuaternion : public WorkspaceTransformation { public: WorkspaceTransformationQuaternion(); bool drawDataFull(DIWNE::Diwne &diwne); int maxLenghtOfData(); };



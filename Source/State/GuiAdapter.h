#pragma once

#include "GUI/Elements/Nodes/WorkspaceTransformation_s.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

template <ETransformType T>
void createTransform(ImVec2 const position)
{
	if constexpr (T == ETransformType::Free)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationFree>(position);
	else if constexpr (T == ETransformType::Translation)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationTranslation>(position);
	else if constexpr (T == ETransformType::EulerX)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationEulerX>(position);
	else if constexpr (T == ETransformType::EulerY)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationEulerY>(position);
	else if constexpr (T == ETransformType::EulerZ)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationEulerZ>(position);
	else if constexpr (T == ETransformType::Scale)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationScale>(position);
	else if constexpr (T == ETransformType::AxisAngle)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationAxisAngle>(position);
	else if constexpr (T == ETransformType::Quat)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationQuaternion>(position);
	else if constexpr (T == ETransformType::Ortho)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationOrthoProjection>(position);
	else if constexpr (T == ETransformType::Perspective)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationPerspective>(position);
	else if constexpr (T == ETransformType::Frustum)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationFrustum>(position);
	else if constexpr (T == ETransformType::LookAt)
		WorkspaceWindow::addNodeToPosition<WorkspaceTransformationLookAt>(position);
}

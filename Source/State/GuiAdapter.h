#pragma once

#include "GUI/Elements/Nodes/WorkspaceTransformation_s.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

template <ETransformType T>
auto createTransform(ImVec2 const position)
{
	if constexpr (T == ETransformType::Free)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationFree>(position);
	else if constexpr (T == ETransformType::Translation)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationTranslation>(position);
	else if constexpr (T == ETransformType::EulerX)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationEulerX>(position);
	else if constexpr (T == ETransformType::EulerY)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationEulerY>(position);
	else if constexpr (T == ETransformType::EulerZ)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationEulerZ>(position);
	else if constexpr (T == ETransformType::Scale)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationScale>(position);
	else if constexpr (T == ETransformType::AxisAngle)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationAxisAngle>(position);
	else if constexpr (T == ETransformType::Quat)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationQuaternion>(position);
	else if constexpr (T == ETransformType::Ortho)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationOrthoProjection>(position);
	else if constexpr (T == ETransformType::Perspective)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationPerspective>(position);
	else if constexpr (T == ETransformType::Frustum)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationFrustum>(position);
	else if constexpr (T == ETransformType::LookAt)
		return WorkspaceWindow::addNodeToPosition<WorkspaceTransformationLookAt>(position);
}

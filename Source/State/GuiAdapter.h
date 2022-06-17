#pragma once

#include "GUI/Elements/Nodes/WorkspaceTransformation_s.h"
#include "GUI/Elements/Windows/WorkspaceWindow.h"

template <ETransformType T>
auto createTransform(ImVec2 const position) /* \todo JH unused now? */
{
	return g_workspaceDiwne->addNodeToPosition<WorkspaceTransformation_s<T>>(position);
	//	if constexpr (T == ETransformType::Free)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationFree>(position);
	//	else if constexpr (T == ETransformType::Translation)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationTranslation>(position);
	//	else if constexpr (T == ETransformType::EulerX)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationEulerX>(position);
	//	else if constexpr (T == ETransformType::EulerY)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationEulerY>(position);
	//	else if constexpr (T == ETransformType::EulerZ)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationEulerZ>(position);
	//	else if constexpr (T == ETransformType::Scale)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationScale>(position);
	//	else if constexpr (T == ETransformType::AxisAngle)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationAxisAngle>(position);
	//	else if constexpr (T == ETransformType::Quat)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationQuaternion>(position);
	//	else if constexpr (T == ETransformType::Ortho)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationOrthoProjection>(position);
	//	else if constexpr (T == ETransformType::Perspective)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationPerspective>(position);
	//	else if constexpr (T == ETransformType::Frustum)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationFrustum>(position);
	//	else if constexpr (T == ETransformType::LookAt)
	//		return WorkspaceWindow::m_workspaceDiwne.addNodeToPosition<WorkspaceTransformationLookAt>(position);
}

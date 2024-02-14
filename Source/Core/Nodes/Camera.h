/**
 * \file
 * \brief
 * \author Martin Herich <martin.herich@phire.cz>
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Node.h"
#include "Sequence.h"

namespace Core
{
class Camera;

namespace Builder
{
Ptr<Camera> createCamera();
}

constexpr unsigned I3T_CAMERA_OUT_SCREEN = 0;
constexpr unsigned I3T_CAMERA_OUT_MATRIX = 1;
constexpr unsigned I3T_CAMERA_OUT_MUL = 2;

/**
 * Camera has following outputs
 * 0 -> screen output
 * 1 -> proj * view matrix output
 * 2 -> mul output
 */
class Camera : public Node
{
	friend class GraphManager;

	Ptr<Sequence> m_proj;
	Ptr<Sequence> m_view;

public:
	/// Latest projection matrix value
	glm::mat4 m_projectionMatrix;
	/// Latest view matrix value
	glm::mat4 m_viewMatrix;

	Camera();

	void createComponents();

	const Ptr<Sequence>& getProj()
	{
		return m_proj;
	}

	const Ptr<Sequence>& getView()
	{
		return m_view;
	}

	void updateValues(int inputIndex) override;

	// void isPlugCorrectMod(const Pin& input, const Pin& output);
};

using CameraPtr = Ptr<Camera>;
} // namespace Core

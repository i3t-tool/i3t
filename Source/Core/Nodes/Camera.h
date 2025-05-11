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

enum class ClipRange
{
	MinusOneToOne,  // Standard OpenGL NDC space (-1, 1)
	OneToMinusOne,  // OpenGL NDC space prior to applying the projection, -z depth
	ZeroToOne,      // Standard Vulkan NDC space (0, 1)
	ZeroToMinusOne, // Vulkan NDC space prior to applying the projection, -z depth
	                // TODO: OneToZero (Reversed) [OGL-VULKAN]
};

enum class DepthRange
{
	ZeroToOne
};

struct CameraCoordSystem
{
	ClipRange clipRange{ClipRange::OneToMinusOne};
	DepthRange depthRange{DepthRange::ZeroToOne};
	bool yUp{true};

	friend bool operator==(const CameraCoordSystem& lhs, const CameraCoordSystem& rhs)
	{
		return lhs.clipRange == rhs.clipRange && lhs.depthRange == rhs.depthRange && lhs.yUp == rhs.yUp;
	}
	friend bool operator!=(const CameraCoordSystem& lhs, const CameraCoordSystem& rhs)
	{
		return !(lhs == rhs);
	}

	bool isLeftHanded() const
	{
		if (clipRange == ClipRange::MinusOneToOne && yUp)
			return true;
		return false;
	}

	/// Whether to flip the X coordinate (of the standard I3T right handed coord system)
	bool isXFlipped() const
	{
		if (clipRange == ClipRange::MinusOneToOne && yUp)
			return true;
		if (clipRange == ClipRange::ZeroToOne && !yUp)
			return true;
		return false;
	}
};

extern CameraCoordSystem g_i3t; ///< The initial I3T specific "world" coordinate system (right handed, view towards -Z)
extern CameraCoordSystem g_openGL; ///< The standard OpenGL coord system
extern CameraCoordSystem g_vulkan; ///< The standard Vulkan coord system

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
	Ptr<Sequence> m_viewport;

public:
	/// Latest projection matrix value
	glm::mat4 m_projectionMatrix{1.f};
	/// Latest view matrix value
	glm::mat4 m_viewMatrix{1.f};
	/// Latest viewport matrix value
	glm::mat4 m_viewportMatrix{1.f};

	/// Whether the viewport transformation should be taken into account (it is optional)
	bool m_viewportEnabled = false;

	/// Coordinate system used during camera tracking
	CameraCoordSystem m_coordinateSystem{g_openGL};
	// TODO: Coordinate system serialization, same goes for relevant transforms

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

	const Ptr<Sequence>& getViewport()
	{
		return m_viewport;
	}

	void updateValues(int inputIndex) override;

	// void isPlugCorrectMod(const Pin& input, const Pin& output);

	bool isEmpty() const; ///< Whether the camera has any transformations in it
};

using CameraPtr = Ptr<Camera>;
} // namespace Core

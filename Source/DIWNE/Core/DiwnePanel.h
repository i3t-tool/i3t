/**
 * \file
 * \brief
 * \author Dan Rakušan <rakusan.dan@gmail.com>
 * \date 4.11.2024
 * \copyright Copyright (C) 2016-2023 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "DiwneObject.h"

namespace DIWNE
{
/**
 * A rectangular area representing a part of a DIWNE object.
 * Used to keep track of additional information about a part of a DIWNE object's content.
 * Note that this class is NOT a subclass of a DiwneObject.
 * It is merely a utility class meant to be used within DIWNE objects.
 *
 * Intended use:
 *
 * 1. Draw content using ImGui, whilst
 * 	  - Springs use getAvailableSpringWidth() to set their size (setting their width to a fraction of available width).
 *    - Each spring will inform the panel of its width by calling submitSpringWidth()
 * 2. When drawing is done, the panels dimensions are set to the dimensions of the drawn content.
 * 	  (Using ImGui Groups and ImGui::GetItemRectXXX())
 * 3. Layout constraints are calculated.
 *    - Panels can be queried with getSpringWidth() to get width they are willing to shrink by.
 *      The method getMinimumWidth() can be used too. It returns width of the node that DOESN'T include spring widths.
 *    - Panels are then resized, whilst updating their available width via setAvailableSpringWidth()
 * 4. Panel's submitted spring width is reset with resetSpringWidth() to start counting springs widths during next
 *    frame.
 */
class DiwnePanel
{
protected:
	NodeEditor& m_editor; ///< Reference to the overarching node editor
	std::string m_label;  ///< ImGui label used for identification

	ImRect m_rect{ImRect()};         ///< Actual rect including any extra spacings that can shrink.
	float m_availableSpringWidth{0}; ///< Total width that can be filled with springs.
	float m_submittedSpringWidth{0}; ///< Actual width filled with springs.

public:
	DiwnePanel(NodeEditor& editor, std::string label);

	void begin();
	void end();

	// TODO: Spring helper
	inline void spring(float relSize) {}

	/**
	 * Mark this width as spring width which can shrink if needed.
	 * Submitted width is added to a cumulative total for the current frame.
	 * Total submitted width SHOULD NOT be more than the available spring width for this panel.
	 * @param width
	 */
	inline void submitSpringWidth(float width)
	{
		m_submittedSpringWidth += width;
	}

	/**
	 * Resets the submitted spring width counter. Should be called AFTER rendering and layouting has been done
	 * and after next frame available spring width for the panel has been set.
	 */
	inline void resetSpringWidth()
	{
		m_submittedSpringWidth = 0;
	}

	/**
	 * Returns this panel's cumulative spring width for the last frame.
	 * That is the portion of width of the panel which can shrink.
	 */
	inline float getSpringWidth() const
	{
		return m_submittedSpringWidth;
	}

	/**
	 * Sets the available spring width which the panel can use for springs during next frame's drawing.
	 */
	inline void setAvailableSpringWidth(float width)
	{
		m_availableSpringWidth = width;
	}
	inline float getAvailableSpringWidth() const
	{
		return m_availableSpringWidth;
	}

	// Higher level modification methods _______________________________

	/**
	 * Sets the width of the panel, extending the rectangle to the right. Or shrinking to the left.
	 * @return The difference between the old and new width (positive if width increased).
	 */
	inline float setWidth(float width)
	{
		assert(width >= 0); // Width needs to be positive (inversion not desirable)
		float prevWidth = getWidth();
		setMaxX(getMinX() + width);
		return width - prevWidth;
	}

	inline void translate(const ImVec2& v)
	{
		m_rect.Translate(v);
	}

	// Panel queries ____________________________________________________

	/**
	 * Returns the real width of the diwne panel.
	 */
	inline float getWidth() const
	{
		return m_rect.GetWidth();
	}
	/**
	 * Returns the minimum width of the panel.
	 * Minimum width does not contain spring widths as those are willing to shrink if necessary.
	 */
	inline float getMinimumWidth()
	{
		return getWidth() - getSpringWidth();
	}

	inline float getHeight() const
	{
		m_rect.GetHeight();
	}
	inline ImVec2 getSize() const
	{
		m_rect.GetSize();
	}

	// Rect modification methods _______________________________________

	inline void setMinX(float x)
	{
		m_rect.Min.x = x;
	}
	inline void setMinY(float y)
	{
		m_rect.Min.y = y;
	}
	inline void setMaxX(float x)
	{
		m_rect.Max.x = x;
	}
	inline void setMaxY(float y)
	{
		m_rect.Max.y = y;
	}
	inline void setMin(const ImVec2& min)
	{
		setMinX(min.x);
		setMinY(min.y);
	}
	inline void setMax(const ImVec2& max)
	{
		setMaxX(max.x);
		setMaxY(max.y);
	}
	inline void setRect(const ImVec2& min, const ImVec2& max)
	{
		setMin(min);
		setMax(max);
	}
	inline void setRect(const ImRect& rect)
	{
		setMin(rect.Min);
		setMax(rect.Max);
	}

	// Rect Getters _____________________________________________________

	inline float getMinX() const
	{
		return m_rect.Min.x;
	}
	inline float getMinY() const
	{
		return m_rect.Min.y;
	}
	inline float getMaxX() const
	{
		return m_rect.Max.x;
	}
	inline float getMaxY() const
	{
		return m_rect.Max.y;
	}
	inline const ImVec2& getMin() const
	{
		return m_rect.Min;
	}
	inline const ImVec2& getMax() const
	{
		return m_rect.Max;
	}
	inline const ImRect& getRect() const
	{
		return m_rect;
	}
};


} // namespace DIWNE

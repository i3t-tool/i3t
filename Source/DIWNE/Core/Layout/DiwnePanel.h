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

#include "DIWNE/Core/Elements/DiwneObject.h"

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
 *
 * DiwnePanels are meant to be a generic layout utility, but then need to work in "stable" coordinates.
 * Hence they use DIWNE coordinates__ which are converted from screen space using the provided editor reference.
 * This incurs an extra runtime cost that wouldn't be necessary if used outside of the NodeEditor.
 * TODO: I might create a separate layout class for general ImGui if DiwnePanels prove useful
 */
class DiwnePanel
{
public:
	enum Mode : char
	{
		FIXED,
		SPRINGY
	};

protected:
	NodeEditor& m_editor; ///< Reference to the overarching node editor
	std::string m_label;  ///< ImGui label used for identification

	// TODO: Wouldn't it make more sense to hold most of these sizes in screen coordinates?
	//  And then convert to diwne when needed.
	//  Right now its the other way around, screen is converted to diwne at all times
	//  But I really feel like we use and convert to screen space more often than diwne
	//  This is an ImGui layouting utility, realistically we only need to convert to diwne
	//  at the end of drawing a node to update m_rect.
	//  ANSWER: No, since when zoom changes the fixed with / spring sizes would become invalid and would need to be
	//  somehow adjusted to prevent a one frame "jump/flicker".
	//  Thus its best if the panel uses the "true"/"stable" DIWNE coordinates
	//   -> but we have to deal with conversion, perhaps additional screen space info could be retained to reduce
	//      necessary conversions.

	ImRect m_rect{ImRect()}; ///< Actual rect including any extra spacings that can shrink. In DIWNE space.

	ImRect m_lastScreenRect{ImRect()}; ///< Last screen space rectangle reported by ImGui. Used in special cases.
	                                   ///< @see getLastScreenRectDiwne()
	// TODO: Panels should perhaps be given a Viewport (canvas?), not the whole

	// NOTE: All the sizes below are in DIWNE coordinates!

	float m_availableSpringWidth{0}; ///< Total width that can be filled with springs.

	float m_submittedFixedWidth{0};  ///< Width filled with non-shrinkable items.
	float m_submittedSpringWidth{0}; ///< Actual width filled with springs.

	bool m_widthQueued{false};
	float m_queuedFixedWidth{0};

	// NOTES:
	// The panel operates in two modes: 1. fixed and 2. springy
	// It can switch between these modes at runtime.
	//
	// 1. Fixed (m_submittedSpringWidth == 0)
	// If the panel does not contain any springs, its minimum size always equals its size.
	// In this mode, tracking of fixed width is not necessary and submitFixedWidth() doesn't need to be called.
	// In the end() method of the panel, it's minimum width is set to its current ImGui width.
	//
	// 2. Springy (m_submittedSpringWidth > 0)
	// When m_submittedSpringWidth is not 0, then it means there is a spring present in the panel
	// That means that the panels "real" m_rect width, is not equal to its minimum width.

	Mode m_mode{FIXED};

public:
	DiwnePanel(NodeEditor& editor, std::string label);

	// Lifecycle
	// =============================================================================================================

	void begin();
	void end(DiwnePanel* parent = nullptr);

	void layout();

	// Widgets
	// =============================================================================================================

	void spring(float relSize);

	void sameLine(float spacing = -1.0f);

	// Layouting
	// =============================================================================================================

	void submitItem(); ///< Submits the last ImGui item as fixed width to the panel.
	void submitItem(const std::shared_ptr<DiwneObject>& item); ///< Submits DiwneObject as fixed with to the panel.
	void submitItem(DiwneObject* item);                        ///< Submits DiwneObject as fixed with to the panel.
	void submitChild(DiwnePanel* child);                       ///< Submits given panel as fixed width to the panel

	/**
	 * Add fixed non-shrinking width to the panel.
	 * @param width Width to submit in diwne coordinates.
	 */
	void submitFixedWidth(float width);

protected:
	/**
	 * When calling ImGui::SameLine() no width is actually added until an ImGui item is submitted.
	 * Hence we can't automatically submit fixed width when DiwnePaneL::sameLine() is called since it may not actually
	 * be "real" width if no other ImGui item is added afterwards.
	 * So we instead queue the width to be submitted later when another item is added. This panel should be informed
	 * about a new item being added with addChild() or spring() calls.
	 */
	void queueFixedWidth(float width);

	void applyQueuedWidth();

	/**
	 * Mark this width as spring width which can shrink if needed.
	 * Submitted width is added to a cumulative total for the current frame.
	 * Total submitted width SHOULD NOT be more than the available spring width for this panel.
	 * @param width Spring width in diwne coordinates.
	 */
	void submitSpringWidth(float width);

public:
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

	// Higher level modification methods
	// =============================================================================================================

	/**
	 * Sets the width of the panel, extending the rectangle to the right. Or shrinking to the left.
	 * @return The difference between the old and new width (positive if width increased).
	 */
	float setWidth(float width);

	inline void translate(const ImVec2& v)
	{
		m_rect.Translate(v);
	}

	// Panel queries
	// =============================================================================================================

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
	 * This width should equal getWidth() - getSpringWidth() (TODO: perhaps add an assert and warning if it doesnt).
	 * But we don't use the m_rect width captured from ImGui here as that width includes spring items and unless
	 * the ImGui measurements are exactly precise there is a risk of a feedback loop expanding the width indefinitely.
	 */
	float getMinimumWidth() const;

	inline float getHeight() const
	{
		return m_rect.GetHeight();
	}
	inline ImVec2 getSize() const
	{
		return m_rect.GetSize();
	}
	Mode getMode() const
	{
		return m_mode;
	}

	// Rect modification methods
	// =============================================================================================================

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

	// Rect Getters
	// =============================================================================================================

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

	/**
	 * Returns the last screen space rectangle of this panel as reported by ImGui in the last end() call.
	 * Can be used in special cases instead of the getRect() method to avoid unnecessary coordinate conversion.
	 *
	 * @warning This rectangle is merely the last computed ImGui screen space rectangle. It does not have to correspond
	 * to getRect() and is only valid immediately after drawing, before layouting. Convert getRect() using screen2diwne
	 * to get a real screen space rect representation.
	 */
	inline const ImRect& getLastScreenRectDiwne() const
	{
		return m_lastScreenRect;
	}

protected:
	// Internal
	// =============================================================================================================
	void reset();
};

} // namespace DIWNE

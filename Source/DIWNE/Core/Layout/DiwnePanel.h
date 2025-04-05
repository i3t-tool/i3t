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
 * Used for layouting, DiwnePanels support horizontal and vertical springs, which can be used to align fixed items
 * within the panel.
 * Note that this class is NOT a subclass of a DiwneObject.
 * It is merely a utility class meant to be used within DIWNE objects.
 *
 * DiwnePanels operate in DIWNE coordinates to be as view/zoom agnostic as possible. Unfortunately view position and
 * zoom level still affects all coordinates due to ImGui pixel alignment.
 *
 * Basic layout concept:
 * DiwnePanel is a rectangle that tracks "fixed" size, which are parts of the rectangle that CANNOT shrink.
 * Remaining size in the rectangle is considered "springy" and may be shrunk. Since the panel is tracking its fixed
 * width (eg. it's minimal width), springs can be added within it that share some remaining available space.
 * So to right align the panels content, a spring can be added to the beginning with a factor of 1.0f, making it use
 * all the available space, right aligning the content. Similarly two springs with 0.5f factor each can be used to
 * surround a fixed item and center it in the panel.
 *
 * TODO: Better docs, examples etc.
 *
 * DiwnePanels are meant to be a generic layout utility, but then need to work in "stable" coordinates.
 * Hence they use DIWNE coordinates, which are converted from screen space using the provided editor reference.
 * This incurs an extra runtime cost that wouldn't be necessary if used outside of the NodeEditor.
 *
 * TODO: I might create a separate layout class for general ImGui if DiwnePanels prove useful
 *
 * DiwnePanels themselves are merely a component of a larger layout system.
 * @see DIWNE::Stack
 */
class DiwnePanel
{
protected:
	NodeEditor& m_editor; ///< Reference to the overarching node editor
	std::string m_label;  ///< ImGui label used for identification

	ImRect m_rect{ImRect()};           ///< Actual rect including any extra spacings that can shrink. In DIWNE space.
	ImRect m_lastScreenRect{ImRect()}; ///< Last screen space rectangle reported by ImGui. Used in special cases.
	                                   ///< @see getLastScreenRectDiwne()
	// TODO: Panels should perhaps be given a Viewport (canvas?), not the whole editor

	// NOTE: All the sizes below are in DIWNE coordinates!

	ImVec2 m_availableSpringSize; ///< Total width/height that can be filled with springs, in DIWNE coordinates.

	int hsprings = 0, vsprings = 0; // TODO: This isn't really used for anything

	ImVec2 m_fixedSize;           ///< Cumulative size of fixed items within the panel. The minimum size.
	ImVec2 m_lastFixedPositionXY; ///< Internal variable for tracking the fixed items size automatically

public:
	DiwnePanel(NodeEditor& editor, std::string label);

	// Lifecycle
	// =============================================================================================================

	void begin();
	// TODO: Do we need to submit anything to a parent? Otherwise the parent considers child spring size to be fixed.
	//  But is that a problem? Because the child doesn't consider it fixed so maybe it all works out in the end?
	//  That would actually be quite nice because there would be no need for any sort of panel hierarchy.
	void end(DiwnePanel* parent = nullptr);

	/// Recalculates the available spring size for the next frame, should be called after end() and after its preferred
	/// dimensions have been set.
	void layout();

	// Layouting widgets
	// =============================================================================================================

	/**
	 * Adds a horizontal (or vertical) spring (dummy item) with width/height calculated based on remaining available
	 * width in the panel.
	 *
	 * @warning Springs operate in a single dimension, eg. in a single line, single "column". The space they span isn't
	 * meant to be occupied by another item. If you, for example, need several horizontal springs above each other, use
	 * multiple DiwnePanels and a layout manager like VStack.
	 *
	 * @param relSize Proportion of the available width/height this spring ends up using
	 * @param horizontal True for a horizontal spring, false for a vertical spring.
	 * @return Whether the spring item was truly added or not.
	 */
	bool spring(float relSize, bool horizontal = true);

	bool vspring(float relSize); ///< Vertical spring @see spring()

	// Layouting commands, use with care
	// =============================================================================================================

	// TODO: Is this needed? See TODO above.
	void submitChild(DiwnePanel* child); ///< Submits given panel as fixed width to the panel

	/// Inform the next spring that a new fixed width item will appear this frame.
	/// Can be used to prevent flickering on layout change.
	void expectWidthChangeThisFrame(float width);

public:
	// /**
	//  * Returns this panel's cumulative spring width for the last frame.
	//  * That is the portion of width of the panel which can shrink.
	//  */
	// inline float getSpringWidth() const
	// {
	// 	return m_submittedSpringWidth;
	// }

	// /**
	//  * Sets the available spring width which the panel can use for springs during next frame's drawing.
	//  */
	// inline void setAvailableSpringWidth(float width)
	// {
	// 	m_availableSpringWidth = width;
	// }

	inline ImVec2 getAvailableSize() const
	{
		return m_availableSpringSize;
	}

	// Panel layout
	// =============================================================================================================

	/**
	 * Returns the minimum width of the panel.
	 * Minimum width does not contain spring widths as those are willing to shrink if necessary.
	 */
	float getMinimumWidth() const;
	/**
	 * Returns the minimum height of the panel.
	 * Minimum height does not contain spring heights as those are willing to shrink if necessary.
	 */
	float getMinimumHeight() const;
	/**
	 * Returns the minimum size of the panel.
	 * Minimum size does not contain spring sizes as those are willing to shrink if necessary.
	 */
	ImVec2 getMinimumSize() const;

	float getMinMaxX() const;
	float getMinMaxY() const;

	// Rect modification methods
	// =============================================================================================================

	/**
	 * Sets the width of the panel, extending the rectangle to the right. Or shrinking to the left.
	 */
	void setWidth(float width);
	void setHeight(float height);

	inline void translate(const ImVec2& v)
	{
		m_rect.Translate(v);
	}

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

	/// Returns the real width of the diwne panel.
	inline float getWidth() const
	{
		return m_rect.GetWidth();
	}
	/// Returns real height of the diwne panel.
	inline float getHeight() const
	{
		return m_rect.GetHeight();
	}

	/// Returns real size of the diwne panel.
	inline ImVec2 getSize() const
	{
		return m_rect.GetSize();
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
	void beginFixedItem();
	void endFixedItem();
};

} // namespace DIWNE

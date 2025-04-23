/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>, Dan Rakušan <rakusan.dan@gmail.com>
 * \copyright Copyright (C) 2016-2025 I3T team, Department of Computer Graphics
 * and Interaction, FEE, Czech Technical University in Prague, Czech Republic
 *
 * This file is part of I3T - An Interactive Tool for Teaching Transformations
 * http://www.i3t-tool.org
 *
 * GNU General Public License v3.0 (see LICENSE.txt or https://www.gnu.org/licenses/gpl-3.0.txt)
 */
#pragma once

#include "Node.h"
#include "State/NodeVisitor.h"

namespace Workspace
{
inline bool CoreNode_useLODIcons = true;
inline bool CoreNode_useDotsForMultiLOD = true;
inline bool CoreNode_shiftLODIcons = true;
inline bool CoreNode_useAngleLODIcon = true;
inline bool CoreNode_useCaretLODIcon = false;
inline bool CoreNode_usePenInBoxIcon = true;

class CoreNode : public Node, public IVisitable
{
	using Super = Node;

protected:
	int m_numberOfVisibleDecimal; ///< number of decimal places used while display floats in the workspace
	float m_dataItemsWidth;
	bool m_updateDataItemsWidth{true};
	// float m_headerMinWidth{0}; ///< Can be used to specify the minimum header width of the node.
	bool m_drawContextMenuButton = false;
	bool m_isLabelBeingEdited = false;
	bool m_isFirstDraw = true;
	bool m_topBottomSpacingDefault = true; ///< Whether this node has spacing between content and header / end.
	FloatPopupMode m_floatPopupMode{FloatPopupMode::Value};
	LevelOfDetail m_levelOfDetail{LevelOfDetail::Full};
	/**
	 * @brief Reference to the I3T Core node
	 * @description Each Workspace GUI node represents a single I3T Core node.
	 * It also manages its lifetime and destruction. The UI and Core representations are not implicitly synced.
	 * Right now the idea is that any changes to the Core nodes is made through Workspace nodes,
	 * which sync the UI and Core state explicitly in the moments of immediate change + some continual asserts.
	 * Workspace is however not ready to react to external changes to the Core node graph and would result in sync fail.
	 */
	const Ptr<Core::Node> m_nodebase;

public:
	constexpr static char CORE_NODE_FLAG = 16; // Index of the CoreNode DIWNE flag (16th bit from the right)

	CoreNode(DIWNE::NodeEditor& diwne, Ptr<Core::Node> nodebase);
	~CoreNode() override;

	// TODO: (DR) Shouldn't this return a reference?
	/// Returns the managed I3T Core node
	Ptr<Core::Node> getNodebase() const;
	const std::string& getKeyword() const; ///< Returns the string identifier of the core node type

	// Lifecycle / Interaction
	// =============================================================================================================
	void begin(DIWNE::DrawInfo& context) override;
	void topContent(DIWNE::DrawInfo& context) override;
	void endDiwne(DIWNE::DrawInfo& context) override;

	void popupContent(DIWNE::DrawInfo& context) override;

	void onDestroy(bool logEvent) override;
	void onPopup() override;

	void onSelection(bool selected) override;

	// Number format / precision
	// =============================================================================================================
	void drawMenuSetEditable();
	void drawMenuSetPrecision();

	int getNumberOfVisibleDecimal();
	virtual void setNumberOfVisibleDecimal(int value);

	FloatPopupMode& getFloatPopupMode();
	void setFloatPopupMode(FloatPopupMode mode);

	/// Returns the desired width of data items (ImGui number fields)
	float getDataItemsWidth();
	/// This call requests the data items width to be recalculated on the next draw of ths node.
	void queueUpdateDataItemsWidth();

protected:
	/// Calculates the final desired pixel width of data items. Calls maxLengthOfData() internally.
	/// Despite this method not being particularly time intensive it is NOT called every frame.
	/// A call to this method can be queued for the next frame via queueUpdateDataItemsWidth()
	/// @see maxLengthOfData(), queueUpdateDataItemsWidth()
	virtual float updateDataItemsWidth();

	/// Calculates the largest number of characters/decimal places a data item has in this node.
	/// This call will often be delegated to the pins of the node.
	virtual int maxLengthOfData() = 0;

public:
	// Level of detail
	// =============================================================================================================
	virtual void drawMenuLevelOfDetail() = 0;
	static void drawMenuLevelOfDetail_builder(Ptr<CoreNode> node, std::vector<LevelOfDetail> const& levels_of_detail);

	LevelOfDetail setLevelOfDetail(LevelOfDetail levelOfDetail);
	LevelOfDetail getLevelOfDetail();

	/// Switches to the next LOD level
	/// @returns Thew new LOD level
	virtual LevelOfDetail switchLevelOfDetail(LevelOfDetail oldLevel);
	virtual int getLODCount(); ///< Number of levels this node cycles through

public:
	// Duplication
	// =============================================================================================================
	void drawMenuDuplicate(DIWNE::DrawInfo& context);
	void duplicate(DIWNE::DrawInfo& context, bool multiDuplication);
	void onReleased(bool justReleased, DIWNE::DrawInfo& context) override;

	// Tracking
	// =============================================================================================================
public:
	void drawTrackingCursor(ImRect rect, const Core::TrackedNodeData* t) const;
	void drawTrackingBorder(bool active, bool interpolating, float progress) const;

	// =============================================================================================================
private:
	void drawLODButton(DIWNE::DrawInfo& context, LevelOfDetail detail, ImDrawFlags cornerFlags);
	const char* getButtonSymbolFromLOD(LevelOfDetail detail);
	void drawContextMenuButton(DIWNE::DrawInfo& context, ImDrawFlags cornerFlags);
	void drawHeaderLabel(DIWNE::DrawInfo& context);
};
} // namespace Workspace
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
class CoreNode : public Node, public IVisitable
{
	using Super = Node;

protected:
	int m_numberOfVisibleDecimal; ///< number of decimal places used while display floats in the workspace
	float m_dataItemsWidth;
	float m_headerMinWidth{0}; ///< Can be used to specify the minimum header width of the node.
	bool m_isLabelBeingEdited = false;
	bool m_isFirstDraw = true;
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

	/// Returns the managed I3T Core node
	Ptr<Core::Node> getNodebase() const;
	const std::string& getKeyword() const;

	// Lifecycle
	// =============================================================================================================
	void topContent(DIWNE::DrawInfo& context) override;

	void popupContent(DIWNE::DrawInfo& context) override;

	void onDestroy(bool logEvent) override;

	// Number format / precision
	// =============================================================================================================
	bool drawDataLabel();
	void drawMenuSetEditable();
	void drawMenuSetPrecision();

	int getNumberOfVisibleDecimal();
	virtual void setNumberOfVisibleDecimal(int value);

	FloatPopupMode& getFloatPopupMode();
	void setFloatPopupMode(FloatPopupMode mode);

	virtual int maxLengthOfData() = 0;

	float getDataItemsWidth();
	virtual float updateDataItemsWidth();

	// Level of detail
	// =============================================================================================================
	virtual void drawMenuLevelOfDetail() = 0;
	static void drawMenuLevelOfDetail_builder(Ptr<CoreNode> node, std::vector<LevelOfDetail> const& levels_of_detail);

	LevelOfDetail setLevelOfDetail(LevelOfDetail levelOfDetail);
	LevelOfDetail getLevelOfDetail();

	// Duplication
	// =============================================================================================================
	void drawMenuDuplicate(DIWNE::DrawInfo& context);
	void duplicate(DIWNE::DrawInfo& context, bool multiDuplication);
	void onReleased(bool justReleased, DIWNE::DrawInfo& context) override;

	// =============================================================================================================

	void onSelection(bool selected) override;

private:
	const char* getButtonSymbolFromLOD(LevelOfDetail detail);
};
} // namespace Workspace
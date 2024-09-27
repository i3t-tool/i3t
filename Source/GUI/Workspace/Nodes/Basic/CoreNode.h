/**
 * \file
 * \brief
 * \author Jaroslav Holeček <holecek.jaroslav@email.cz>
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
#include "State/NodeVisitor.h"

namespace Workspace
{
class CoreNode : public Node, public IVisitable
{
protected:
	int m_numberOfVisibleDecimal; ///< number of decimal places used while display floats in the workspace
	float m_dataItemsWidth;
	float m_headerMinWidth{0}; ///< Can be used to specify the minimum header width of the node.
	bool m_isLabelBeingEdited = false;
	bool m_isFirstDraw = true;
	FloatPopupMode m_floatPopupMode;
	LevelOfDetail m_levelOfDetail;

	/**
	 * \brief reference to Core
	 *
	 * WorkspaceNodeWithCoreData is owner
	 */
	Ptr<Core::Node> const m_nodebase;

public:
	CoreNode(DIWNE::Diwne& diwne, Ptr<Core::Node> nodebase);
	~CoreNode() override;

	Ptr<Core::Node> const getNodebase() const;

	int getNumberOfVisibleDecimal();
	virtual void setNumberOfVisibleDecimal(int value);

	FloatPopupMode& getFloatPopupMode()
	{
		return m_floatPopupMode;
	};
	void setFloatPopupMode(FloatPopupMode mode)
	{
		m_floatPopupMode = mode;
	};

	virtual int maxLengthOfData() = 0;

	float getDataItemsWidth();
	virtual float updateDataItemsWidth();
	bool getIsLabelBeingEdited()
	{
		return m_isLabelBeingEdited;
	};

	LevelOfDetail setLevelOfDetail(LevelOfDetail levelOfDetail);
	LevelOfDetail getLevelOfDetail();

	bool drawDataLabel();
	void drawMenuSetEditable();

	/* DIWNE function */
	bool topContent() override;

	virtual void drawMenuLevelOfDetail() = 0;
	static void drawMenuLevelOfDetail_builder(Ptr<CoreNode> node, std::vector<LevelOfDetail> const& levels_of_detail);

	void drawMenuSetPrecision();

	void popupContent() override;

	bool processDrag() override;
	bool processSelect() override;
	bool processUnselect() override;

	// TODO: (DR) Figure out what this was about
	// TODO: (DR) Mouse buttons are "hard-coded" in DiwneObject, presumably JH was
	//  trying to hook them up to
	//  the InputManager. But that change was only made here and not in the
	//  DiwneObject superclass causing inconsistent behaviour. So I commented these
	//  overrides out for the time being

	// bool bypassDragAction();
	// bool bypassHoldAction();
	// bool bypassUnholdAction();
	// bool bypassSelectAction();
	// bool bypassUnselectAction();
	// bool bypassTouchAction();
	void drawMenuDuplicate();

private:
	const char* getButtonSymbolFromLOD(LevelOfDetail detail);
};
} // namespace Workspace
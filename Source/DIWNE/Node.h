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

#include "diwne_include.h"

namespace DIWNE
{
class Pin;

/**
 */

/*! \brief Used when one node is inside of another -> the inner node is drawn
 * OnCursorPosition when outer node is drawn
 */
enum DrawModeNodePosition
{
	OnCursorPosition, ///< cursor position in screen coordinates
	OnItsPosition     ///< node position in Diwne coordinates
};

/**
 * \brief graphical representation od a box in the workspace node editor
 *
 *  Parts of the Node:
 * - -------------------
 * - |      Top        |
 * - -------------------
 * - |Left|Middle|Right|    -> |L|M|R| == Center
 * - -------------------
 * - |     Bottom      |
 * - -------------------
 */
class Node : public DiwneObject
{
public:
	/** Default constructor */
	Node(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne = "DiwneNode");
	/** Default destructor */
	virtual ~Node();

	//        /** Copy constructor
	//         *  \param other Object to copy from
	//         */
	//        Node(const Node& other);

	/** Assignment operator
	 *  \param other Object to assign from
	 *  \return A reference to this
	 */
	Node& operator=(const Node& other);

	DIWNE::ID const getId() const
	{
		return m_idDiwne;
	};

	DIWNE::DiwneAction getHoldActionType() const final
	{
		return DiwneAction::HoldNode;
	};
	DIWNE::DiwneAction getDragActionType() const final
	{
		return DiwneAction::DragNode;
	};
	DIWNE::DiwneAction getTouchActionType() const final
	{
		return DiwneAction::TouchNode;
	};

	void updateSizes() override;
	void deleteActionDiwne();
	virtual void deleteAction(){};

	bool allowDrawing() override;
	bool beforeBeginDiwne() override;
	void begin() override;
	bool content() override;
	void end() override;
	bool afterEndDiwne() override;

	template <typename T>
	bool drawNodeDiwne(DrawModeNodePosition nodePosMode = DrawModeNodePosition::OnItsPosition,
	                   DrawMode drawMode = DrawMode::Interacting)
	{
		m_nodePosMode = nodePosMode;
		m_drawMode = drawMode;


		if (!getRender()) // hide the node and its input wire in the tutorial scene
			return false;

		bool interaction_happen = drawDiwne(drawMode);

		if (interaction_happen && !m_toDelete)
		{
			diwne.setLastActiveNode<T>(std::static_pointer_cast<T>(shared_from_this()));
			if (diwne.getDiwneActionActive() == DiwneAction::None ||
			    diwne.getDiwneActionActive() == DiwneAction::InteractingContent /* no specific action */
			)
			{
				diwne.setDiwneAction(DiwneAction::InteractingContent);
			}
		}

		return interaction_happen;
	}

	bool topContentDiwne();
	bool leftContentDiwne();
	bool middleContentDiwne();
	bool rightContentDiwne();
	bool bottomContentDiwne();

	virtual ImRect getRectDiwne() const override
	{
		return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);
	};

	bool setSelected(const bool selected) override;

	bool processSelect() override;
	bool processUnselect() override;

	bool processDrag() override;

	virtual bool topContent();
	virtual bool leftContent();
	virtual bool middleContent();
	virtual bool rightContent();
	virtual bool bottomContent();

	void setNodePositionDiwne(ImVec2 const& position)
	{
		m_nodePositionDiwne = position;
		setNodeRectsPositionDiwne(position);
	};
	ImVec2 getNodePositionDiwne() const
	{
		return m_nodePositionDiwne;
	};
	void translateNodePositionDiwne(ImVec2 const amount)
	{
		m_nodePositionDiwne += amount;
		translateNodeRectsDiwne(amount);
	};

	ImRect getNodeRectDiwne() const
	{
		return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);
	};
	ImVec2 getNodeRectSizeDiwne() const
	{
		return m_bottomRectDiwne.Max - m_topRectDiwne.Min;
	};

	bool getRender() const
	{
		return m_render;
	};
	void setRender(bool render)
	{
		m_render = render;
	};

	bool m_drawAnyway; /*!< you have to draw the node anyway. // (PF) was float!?!?
	                         For example in the first frame after you created it
	                         -> to obtain its real size */
	// todo (PF) rename to something like m_forceToDraw, m_forceFirstTimeDraw, or m_forceToDrawFirstTime
	// This variable is
	// - set to true in the Node constructor only
	// - dropped to false in Node::beforeBeginDiwne()
	// - used to force drawing the node for the first time

protected:
	ImVec2 m_nodePositionDiwne; /* cursor position or a stored node position - can be public */

	/** \brief Rectangle of parts of node in diwne,
	 * are computed every frame based on node content and m_nodePositionDiwne.
	 * Note that these rects are using "world" node editor coordinates, not scaled and translated screen ones.
	 */
	ImRect m_topRectDiwne, m_leftRectDiwne, m_middleRectDiwne, m_rightRectDiwne, m_bottomRectDiwne;
	float m_centerDummySpace; ///< indent value to center the middle part

	DrawModeNodePosition m_nodePosMode;
	bool m_toDelete;      ///< Set to true after node delete action
	bool m_render = true; ///< used to hide nodes in tutorial scenes (action "toggleNodeWorkspaceVisibility")

private:
	void setNodeRectsPositionDiwne(ImVec2 const& position);
	void translateNodeRectsDiwne(ImVec2 const& distance);
};

} /* namespace DIWNE */

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

// #include "DiwneObject.h"

#include "NodeEditor.h"

namespace DIWNE
{

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
protected:
	// TODO: Use m_rect instead
	ImVec2 m_nodePositionDiwne; /* cursor position or a stored node position - can be public */

	DrawModeNodePosition m_nodePosMode{OnItsPosition};
	bool m_toDelete = false; ///< Set to true after node delete action
	bool m_render = true;    ///< used to hide nodes in tutorial scenes (action "toggleNodeWorkspaceVisibility")

public:
	bool m_drawAnyway = true; /*!< you have to draw the node anyway. // (PF) was float!?!?
	                         For example in the first frame after you created it
	                         -> to obtain its real size */
	// todo (PF) rename to something like m_forceToDraw, m_forceFirstTimeDraw, or m_forceToDrawFirstTime
	// This variable is
	// - set to true in the Node constructor only
	// - dropped to false in Node::beforeBeginDiwne()
	// - used to force drawing the node for the first time

	Node(NodeEditor& diwne, std::string labelDiwne = "DiwneNode");

	~Node() override;

	// TODO: Why was copy constuctor commented out? Is it ever used?
	//        /** Copy constructor
	//         *  \param other Object to copy from
	//         */
	//        Node(const Node& other);

	// TODO: Does this operator make sense?
	/** Assignment operator
	 *  \param other Object to assign from
	 *  \return A reference to this
	 */
	Node& operator=(const Node& other);

	//	void updateLayout() override;

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override;
	void end(DrawInfo& context) override;

protected:
	void afterDrawDiwne(DrawInfo& context) override;

public:
	bool allowDrawing() override;

	// TODO: Do we really need a "special" method for drawing nodes? Kinda makes inheritance pointless
	//  Why not move the handling into the NodeEditor content method?
	//  What does a node care about last diwneAction, that should be part of the editor context!
	//  DiwneActions will probably be reworked anyway so take that into account
	template <typename T>
	void drawNodeDiwne(DrawInfo& context, DrawModeNodePosition nodePosMode = DrawModeNodePosition::OnItsPosition,
	                   DrawMode drawMode = DrawMode::Interacting)
	{
		m_nodePosMode = nodePosMode;

		if (!getRender()) // hide the node and its input wire in the tutorial scene
			return;

		DrawInfo drawResult = drawDiwneEx(context);

		if (drawResult.interacted && !m_toDelete)
		{
			diwne.setLastActiveNode<T>(std::static_pointer_cast<T>(shared_from_this()));
			if (diwne.getDiwneActionActive() == DiwneAction::None ||
			    diwne.getDiwneActionActive() == DiwneAction::InteractingContent // no specific action
			)
			{
				diwne.setDiwneAction(DiwneAction::InteractingContent);
			}
		}
	}

	// TODO: I'm pretty sure this is dumb, why use two separate delete flags for base and derived nodes?? <<<<<<<<<<
	// TODO: Investigate usage of these methods
	void deleteActionDiwne();
	virtual void deleteAction(){};

	bool setSelected(const bool selected) override;

	//	bool processSelect() override;
	//	bool processUnselect() override;
	//
	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	void setNodePositionDiwne(ImVec2 const& position)
	{
		m_nodePositionDiwne = position;
		setPosition(position);
	};
	ImVec2 getNodePositionDiwne() const
	{
		return m_nodePositionDiwne;
	};
	void move(ImVec2 const amount)
	{
		m_nodePositionDiwne += amount;
		translate(amount);
	};

	bool getRender() const
	{
		return m_render;
	};
	void setRender(bool render)
	{
		m_render = render;
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
};

} /* namespace DIWNE */

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
#ifndef NODE_H
#define NODE_H

#include "diwne_include.h"

namespace DIWNE
{
class Pin;

/** Parts of Node:
 *  -------------------
 *  |      Top        |
 *  -------------------
 *  |Left|Middle|Right|    -> |L|M|R| == Center
 *  -------------------
 *  |     Bottom      |
 *  -------------------
 */

/*! \brief Used when one node is inside of other -> inner node is drawn
 * OnCoursorPosition when outer node is draw
 */
enum DrawModeNodePosition
{
	OnCoursorPosition,
	OnItsPosition
};

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

	void updateSizes();
	void deleteActionDiwne();
	virtual void deleteAction(){};

	virtual bool allowDrawing();
	virtual bool beforeBeginDiwne();
	virtual void begin();
	virtual bool content();
	virtual void end();
	virtual bool afterEndDiwne();

	template <typename T>
	bool drawNodeDiwne(DrawModeNodePosition nodePosMode = DrawModeNodePosition::OnItsPosition,
	                   DrawMode drawMode = DrawMode::Interacting)
	{
		m_nodePosMode = nodePosMode;
		m_drawMode = drawMode;


		if (!getRender())
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

	virtual ImRect getRectDiwne() const
	{
		return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);
	};

	virtual bool processSelect();
	virtual bool processUnselect();

	virtual bool processDrag();

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

	ImRect getNodeRectDiwne()
	{
		return ImRect(m_topRectDiwne.Min, m_bottomRectDiwne.Max);
	};
	ImVec2 getNodeRectSizeDiwne()
	{
		return m_bottomRectDiwne.Max - m_topRectDiwne.Min;
	};

	bool getSelected() const
	{
		return m_selected;
	};
	void setSelected(bool selected)
	{
		m_selected = selected;
	};

	bool getRender()
	{
		return m_render;
	};
	void setRender(bool render)
	{
		m_render = render;
	};

	float m_drawAnywhere; /*!< you have to draw node anywhere for example in first
	                         frame after you created it -> for obtain its real
	                         size */

protected:
	ImVec2 m_nodePositionDiwne; /* can be public */

	/* Border rects of node - are computed every frame based on node content and
	 * m_nodePositionDiwne */
	ImRect m_topRectDiwne, m_leftRectDiwne, m_middleRectDiwne, m_rightRectDiwne,
	    m_bottomRectDiwne; /*! \brief Rectangle of parts of node in diwne */

	float m_centerDummySpace;
	DrawModeNodePosition m_nodePosMode;
	bool m_toDelete; ///< Set to true after node delete action
	bool m_render = true;

private:
	void setNodeRectsPositionDiwne(ImVec2 const& position);
	void translateNodeRectsDiwne(ImVec2 const& amount);
};

} /* namespace DIWNE */

#endif // NODE_H

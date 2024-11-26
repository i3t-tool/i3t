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

#include "DiwneObject.h"

namespace DIWNE
{
class Pin : public DiwneObject
{
public:
	/** Default constructor */
	Pin(DIWNE::NodeEditor& diwne, std::string const labelDiwne = "DiwnePin");

	void begin(DrawInfo& context) override;
	void content(DrawInfo& context) override{};
	void end(DrawInfo& context) override;
	void updateLayout(DrawInfo& context) override;
	//	bool processInteractionsAlways(DrawInfo& context) override; //TODO: Uncomment

	//	bool processDrag() override;

	virtual const ImVec2& getLinkConnectionPointDiwne()
	{
		return m_connectionPointDiwne;
	};

	/*! \brief Wrapper is run when new link is created and goal pin is hovered but
	 * action not released yet
	 *
	 * \return virtual bool
	 *
	 */
	virtual bool processPin_Pre_ConnectLinkDiwne();
	virtual bool bypassPinLinkConnectionPreparedAction();
	virtual bool allowProcessPin_Pre_ConnectLink();

	virtual bool processConnectionPrepared(); /*!< your content/actions when new link hovered
	                                             goal pin but not released yet */

	virtual void setConnectionPointDiwne(const ImVec2 value)
	{
		m_connectionPointDiwne = value;
	};

	DIWNE::DiwneAction getHoldActionType() const final
	{
		return DiwneAction::HoldPin;
	};
	DIWNE::DiwneAction getDragActionType() const final
	{
		return DiwneAction::DragPin;
	};
	DIWNE::DiwneAction getTouchActionType() const final
	{
		return DiwneAction::TouchPin;
	};

	bool allowPopup() const override;

protected:
	ImVec2 m_connectionPointDiwne; /*!< point of link connection to this pin (wire start or end) */

	virtual void updateConnectionPointDiwne()
	{
		m_connectionPointDiwne = m_rect.GetCenter();
	} /*!< intended to use when Pin is drawn (use properties dependent on drawing)
	     - setConnectionPointDiwne is "hard" setting */
};

} /* namespace DIWNE */

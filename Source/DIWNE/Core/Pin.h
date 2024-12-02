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

	void onDrag(DrawInfo& context, bool dragStart, bool dragEnd) override;

	virtual const ImVec2& getLinkConnectionPointDiwne()
	{
		return m_connectionPointDiwne;
	};

	 /**
	  * Call when the mouse is hovering over the pin and when it is actually released over the pin.
	  * The method is responsible for the underlying plug in logic and determining whether the connection is valid.
	  * @param hovering True when the mouse is only hovering and the pin shouldn't be plugged in yet.
	  */
	virtual void onPlug(bool hovering);

	/**
	 * A condition for starting and receiving a link connection.
	 * Can be used to specify an area where the pin can be dragged from or a link dropped at.
	 */
	virtual bool allowConnection() const;

	virtual void setConnectionPointDiwne(const ImVec2 value)
	{
		m_connectionPointDiwne = value;
	};

	bool allowPopup() const override;
	void processInteractions(DrawInfo& context) override;
	bool allowDrag() const override;

protected:
	ImVec2 m_connectionPointDiwne; /*!< point of link connection to this pin (wire start or end) */

	virtual void updateConnectionPointDiwne()
	{
		m_connectionPointDiwne = m_rect.GetCenter();
	} /*!< intended to use when Pin is drawn (use properties dependent on drawing)
	     - setConnectionPointDiwne is "hard" setting */
};

} /* namespace DIWNE */

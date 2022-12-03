#ifndef PIN_H
#define PIN_H

#include "diwne_include.h"

namespace DIWNE
{
class Pin : public DiwneObject
{
public:
	/** Default constructor */
	Pin(DIWNE::Diwne& diwne, DIWNE::ID id, std::string const labelDiwne = "DiwnePin");
	/** Default destructor */
	virtual ~Pin(){};

	DIWNE::ID const getId() const { return m_idDiwne; };

	virtual bool content()
	{
		ImGui::TextUnformatted(fmt::format("{} content", m_labelDiwne).c_str());
		return false;
	};

	DIWNE::DiwneAction getHoldActionType() const final { return DiwneAction::HoldPin; };
	DIWNE::DiwneAction getDragActionType() const final { return DiwneAction::DragPin; };
	DIWNE::DiwneAction getTouchActionType() const final { return DiwneAction::TouchPin; };

	virtual void begin();
	virtual void end();
	virtual void updateSizes();
	virtual bool processInteractionsAlways();

	virtual const ImVec2& getLinkConnectionPointDiwne() { return m_connectionPointDiwne; };

	virtual bool processDrag();

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

	virtual void setConnectionPointDiwne(ImVec2 value) { m_connectionPointDiwne = value; };

	virtual ImRect getRectDiwne() const { return m_pinRectDiwne; };

protected:
	ImRect m_pinRectDiwne;
	ImVec2 m_connectionPointDiwne; /*!< point of link connection to this pin */

	virtual void updateConnectionPointDiwne()
	{
		m_connectionPointDiwne = m_pinRectDiwne.GetCenter();
	} /*!< intended to use when Pin is drawn (use properties dependent on drawing)
	     - setConnectionPointDiwne is "hard" setting */
};

} /* namespace DIWNE */

#endif // PIN_H

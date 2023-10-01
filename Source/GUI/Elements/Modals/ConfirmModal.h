/**
 * \author Martin Herich
 * \date 25.09.2023
 */
#pragma once

#include <string>

#include "GUI/Elements/IWindow.h"


static const std::string BEFORE_EXIT_MODAL_TITLE = "Save current scene?";
static const std::string BEFORE_EXIT_MODAL_MESSAGE = "Any unsaved content will be discarded, do you want to save it?";

void askBeforeExitScene(std::function<void()> action);

class ConfirmModalStrategy
{
	friend class ConfirmModal;

public:
	ConfirmModalStrategy(std::string title, std::string message, std::string yesText = "Yes", std::string noText = "No")
	    : m_title(title), m_message(message), m_yesText(yesText), m_noText(noText)
	{}

	virtual void beforeOpen() {}

	virtual void onYes() {}

	virtual void onNo() {}

private:
	std::string m_title;
	std::string m_message;
	std::string m_yesText;
	std::string m_noText;

protected:
	/// Non-owning pointer to the modal window set by ConfirmModal to point to itself.
	class ConfirmModal* m_modal = nullptr;
};

class ConfirmModal : public ModalWindow
{
public:
	ConfirmModal(std::unique_ptr<ConfirmModalStrategy> strategy);
	~ConfirmModal() override;

	void onImGui() override;

	static inline bool s_isOpen = false;

private:
	std::unique_ptr<ConfirmModalStrategy> strategy;
};

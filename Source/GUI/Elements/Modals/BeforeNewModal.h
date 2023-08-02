/**
 * \file GUI/Elements/Modals/BeforeCloseModal.h
 * \author Martin Herich
 * \date 25.10.2020
 */
#pragma once

#include "GUI/Elements/IWindow.h"

class BeforeNewModal : public ModalWindow
{
public:
	BeforeNewModal() : ModalWindow("Create new project?") {}

	/**
	 * Show Close? modal centered in main window.
	 */
	void onImGui() override;
};

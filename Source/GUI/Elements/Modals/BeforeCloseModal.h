/**
 * \file GUI/Elements/Modals/BeforeCloseModal.h
 * \author Martin Herich
 * \date 25.10.2020
 */
#pragma once

#include "GUI/Elements/IWindow.h"

class BeforeCloseModal : public ModalWindow
{
public:
	BeforeCloseModal() : ModalWindow("Close?") {}

	/**
	 * Show Close? modal centered in main window.
	 */
	void onImGui() override;
};

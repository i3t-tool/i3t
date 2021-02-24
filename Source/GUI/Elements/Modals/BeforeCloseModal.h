/**
 * \file GUI/Elements/Modals/BeforeCloseModal.h
 * \author Martin Herich
 * \date 25.10.2020
 */
#pragma once

#include "GUI/Elements/IWindow.h"

class BeforeCloseModal : public IWindow
{
public:
	static constexpr const char id[] = "BeforeCloseModal";

	BeforeCloseModal(){};

	/**
	 * Show Close? modal centered in main window.
	 */
	void render() override;
};

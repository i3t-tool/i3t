/**
 * \file	descriptionDialog.h
 *
 * \author Martin Herich, hericmar@fel.cvut.cz
 * \date 29.09.2020
 */
#pragma once

#include "GUI/Elements/IWindow.h"

/**
 * Scene description dialog.
 * \todo Create scene description here.
 * \todo Make text resizable.
 * \todo Make open scene unsaved after text editing.
 */
class DescriptionDialog : public IWindow
{
public:
	static constexpr const char id[] = "DescriptionDialog";

	DescriptionDialog();
	virtual void render() override;

private:
	bool m_isEditState;

	/// Description text has fixed length.
	char m_descriptionText[8096] = "";
};

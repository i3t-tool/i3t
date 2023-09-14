/**
 * \file GUI/Elements/Modals/RemoveModelModal.h
 * \author Dan Rakušan
 * \date 14.09.2023
 */
#pragma once

#include <functional>

#include "GUI/Elements/IWindow.h"

class RemoveModelModal : public ModalWindow
{
private:
	std::string m_modelAlias;
	int m_usedCount;

	std::function<void()> m_onRemove;

public:
	RemoveModelModal(std::string modelAlias, int usedCount, std::function<void()> onRemove);

	void onImGui() override;
};

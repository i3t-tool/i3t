#pragma once

#include <vector>

#include "DiwnePanel.h"

namespace DIWNE
{
class VStack
{
	NodeEditor& m_editor;
	std::vector<DiwnePanel> m_children;

	bool m_buildingRow = false; // Make this a DIWNE_DEBUG variable
	bool m_buildingStack = false; // Make this a DIWNE_DEBUG variable

	int m_currentRowIndex = 0;

	float m_maxFixedWidth = 0;


	ImVec2 origin;

public:
	explicit VStack(NodeEditor& editor) : m_editor(editor) {}

	void begin();
	void end();

	DiwnePanel* beginRow();
	void endRow();

private:
	// Probably called by end()
	void layout();
};

} // namespace DIWNE

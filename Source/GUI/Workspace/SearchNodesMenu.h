#pragma once

#include <string>
#include <vector>
#include <functional>
#include <imgui.h>

struct SearchableNode
{
	std::string name;
	std::string category;
	std::function<void()> spawnFunc;
};

class SearchNodesMenu
{
public:
	static SearchNodesMenu& getInstance()
	{
		static SearchNodesMenu instance;
		return instance;
	}

	void init();                 
	void render(ImVec2 spawnPos);
	void open();                  

private:
	SearchNodesMenu() = default;

	std::vector<SearchableNode> m_availableNodes;
	std::string m_searchQuery = "";
	int m_selectedIndex = 0;
	bool m_wantsToOpen = false;
};


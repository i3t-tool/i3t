#pragma once

#include <string>

namespace DIWNE {
struct Actions {
	inline static const std::string selectionRect = "selectionRect";

	struct SelectionRectData {
		bool touch = false;
	};
};
}
#pragma once

#include <string>

namespace DIWNE {
struct Actions {
	inline static const std::string selectionRect = "selectionRect";
	inline static const std::string connectPin = "connectPin";

	struct SelectionRectData {
		bool touch = false;
	};
};
}
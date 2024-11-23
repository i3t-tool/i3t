#pragma once

#include "DIWNE/Core/diwne_imgui.h"

namespace DIWNE
{
class InputAdapter
{
public:
	virtual bool bypassIsItemClicked0();
	virtual bool bypassIsItemClicked1();
	virtual bool bypassIsItemClicked2();
	virtual bool bypassIsMouseDown0();
	virtual bool bypassIsMouseDown1();
	virtual bool bypassIsMouseDown2();
	virtual ImVec2 bypassMouseClickedPos0();
	virtual ImVec2 bypassMouseClickedPos1();
	virtual ImVec2 bypassMouseClickedPos2();
	virtual bool bypassIsMouseClicked0();
	virtual bool bypassIsMouseClicked1();
	virtual bool bypassIsMouseClicked2();
	virtual bool bypassIsMouseReleased0();
	virtual bool bypassIsMouseReleased1();
	virtual bool bypassIsMouseReleased2();
	virtual bool bypassIsMouseDragging0();
	virtual bool bypassIsMouseDragging1();
	virtual bool bypassIsMouseDragging2();
	virtual ImVec2 bypassGetMouseDragDelta0();
	virtual ImVec2 bypassGetMouseDragDelta1();
	virtual ImVec2 bypassGetMouseDragDelta2();
	virtual ImVec2 bypassGetMouseDelta();
	virtual ImVec2 bypassGetMousePos();
	virtual float bypassGetMouseWheel();
};

} // namespace DIWNE

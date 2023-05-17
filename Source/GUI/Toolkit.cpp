#include "Toolkit.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Utils/Format.h"

namespace GUI
{
struct InputTextCallback_UserData
{
	std::string* Str;
	ImGuiInputTextCallback ChainCallback;
	void* ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
	InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
	if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
	{
		// Resize string callback
		// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to
		// what we want.
		std::string* str = user_data->Str;
		IM_ASSERT(data->Buf == str->c_str());
		str->resize(data->BufTextLen);
		data->Buf = (char*)str->c_str();
	}
	else if (user_data->ChainCallback)
	{
		// Forward to user callback, if any
		data->UserData = user_data->ChainCallbackUserData;
		return user_data->ChainCallback(data);
	}
	return 0;
}

//

bool InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback,
               void* user_data)
{
	IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
	flags |= ImGuiInputTextFlags_CallbackResize;

	InputTextCallback_UserData cb_user_data;
	cb_user_data.Str = str;
	cb_user_data.ChainCallback = callback;
	cb_user_data.ChainCallbackUserData = user_data;

	const auto formattedLabel = "##" + std::string(label);

	return ImGui::InputText(formattedLabel.c_str(), (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback,
	                        &cb_user_data);
}

void ToggleButton(const char* label, bool& toggled, ImVec2 size)
{
	int colorsPushed = 0;
	if (toggled)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, I3T::getTheme().get(EColor::ActiveColor));
		colorsPushed++;
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, I3T::getTheme().get(EColor::TutorialButtonBg));
		colorsPushed++;
	}
	if (ImGui::Button(label, size))
	{
		toggled = !toggled;
	}
	ImGui::PopStyleColor(colorsPushed);
	colorsPushed = 0;
}

glm::vec2 imToGlm(const ImVec2& v) { return glm::vec2(v.x, v.y); }
glm::vec4 imToGlm(const ImVec4& v) { return glm::vec4(v.x, v.y, v.z, v.w); }

ImVec2 glmToIm(const glm::vec2& v) { return ImVec2(v.x, v.y); }
ImVec4 glmToIm(const glm::vec4& v) { return ImVec4(v.x, v.y, v.z, v.w); }

void drawCross(glm::vec2 pos, ImDrawList* drawList, float thickness, float size, ImColor color)
{
	drawList->AddRectFilled(ImVec2(pos.x - floor(thickness / 2), pos.y - floor(size / 2)),
	                        ImVec2(pos.x + ceil(thickness / 2), pos.y + ceil(size / 2)), color);
	drawList->AddRectFilled(ImVec2(pos.x - floor(size / 2), pos.y - floor(thickness / 2)),
	                        ImVec2(pos.x + ceil(size / 2), pos.y + ceil(thickness / 2)), color);
}

void drawEllipse(float cx, float cy, float rx, float ry, int num_segments, ImDrawList* drawList, ImColor color,
                 float thickness)
{
	// Adapted from https://stackoverflow.com/a/34735255/3452003
	std::vector<ImVec2> points;
	points.reserve(num_segments);

	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta); // precalculate the sine and cosine
	float s = sinf(theta);
	float t;

	float x = 1; // we start at angle = 0
	float y = 0;

	for (int ii = 0; ii < num_segments; ii++)
	{
		// apply radius and offset
		points.push_back({x * rx + cx, y * ry + cy});

		// apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}

	drawList->AddPolyline(&points[0], points.size(), color, ImDrawFlags_Closed, thickness);
}

} // namespace GUI

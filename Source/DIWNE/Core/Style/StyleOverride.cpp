#include "StyleOverride.h"

#include "StyleBase.h"

namespace DIWNE
{

const ImVec4& StyleOverride::color(Style::Var key) const
{
	return get<ImVec4>(key);
}
ImVec2 StyleOverride::size(Style::Var key) const
{
	const StyleBase::StyleEntry* entry = nullptr;
	ImVec2 ret = getImpl<ImVec2>(key, &entry);
	if (entry->dpiScaled)
		ret *= editor->styleBase().dpiScale;
	return ret;
}
float StyleOverride::decimal(Style::Var key) const
{
	const StyleBase::StyleEntry* entry = nullptr;
	float ret = getImpl<float>(key, &entry);
	if (entry->dpiScaled)
		ret *= editor->styleBase().dpiScale;
	return ret;
}
int StyleOverride::integer(Style::Var key) const
{
	return get<int>(key);
}
bool StyleOverride::boolean(Style::Var key) const
{
	return get<bool>(key);
}

} // namespace DIWNE
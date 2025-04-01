#pragma once
namespace DIWNE
{
class DiwnePanel;

// TODO: Not sure if this class is necessary
//  Question still remains, should layouts *not* be panels?
//  And another thing, how to put existing named panel references into a layout?
//  Then I can replace Gravity in BasicNode
class Layout
{
protected:
	DiwnePanel* m_panel{nullptr}; ///< Panel associated with this layout

public:
	Layout() = default;
	explicit Layout(DiwnePanel* panel) : m_panel(panel) {}
};

} // namespace DIWNE

#include "gtest/gtest.h"

#include <algorithm>
#include <functional>

#include "DIWNE/diwne_include.h"

using namespace DIWNE;

TEST(InteractionStateTest, ActionTest)
{
	// TODO: Test that onFrameEnd and onEnd is called properly
	// TODO: Test that action is ended automatically when its source gets deleted/destroyed
	//    - Test when an object is destroyed and unallocated
	//    - But also when its just destroyed, removed from container but still has a hanging reference
	// TODO: Test that action ends at the end of the NEXT frame, not the one it was ended in.
}

TEST(InteractionStateTest, DragTest)
{
	// TODO: Test that drag is ended automatically when its source gets deleted/destroyed (much like action)
	//    - Test when an object is destroyed and unallocated
	//    - But also when its just destroyed, removed from container but still has a hanging reference
}
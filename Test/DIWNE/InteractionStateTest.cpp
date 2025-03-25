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

	// TODO: Test that drag uses the mouse offset of the correct frame (take the delta from the frame the action began)
	// TODO: Test that onFrameEnd() does NOT get called ON and AFTER the frame in which the drag action ended

	// TODO: The reference below is actually WRONG, we WANT to update drag positions at the beginning of the next frame
	//   because then we are working with the latest polled input, instead of input that is one frame old

	//     Drawn frame: |      F1       |       F2       |   F3
	// Object ID drawn: |[P1][RRRR] *   S[P2]*[RRRR]     S
	// Frame on screen: |      F0   ^   |    ^  F1       |   F2
	//                      .       |        |           .
	//                      .       |        |           .
	//                      .   If use input from P1 to change node positions here AFTER drawing,
	//                      .     the change will be first shown in F2
	//                      .                |           .
	//                      .                |           .
	//                      .            But if I update node positions here BEFORE drawing using P2 input, the change
	//                      .              will be rendered immediately after and displayed at the next buffer swap
	//                      .                .           .
	//                   Input delay if F2 uses input from P1
	//                      |<-------------------------->|
	//                                       .           .
	//                   Input delay if F2 uses input from P2
	//                                       |<--------->|
	//
	// F = Frame
	// P(N) = Polling of input for frame number
	// R = Rendering of the frame
	// S = Swapping of frame buffers
	//
	// Frame N should use inputs polled RIGHT before its being rendered (PN).
	// Hence it is not a good idea to use inputs during a frame AFTER its been rendered.
	// To react to input after rendering,
	// wait until the beginning of the NEXT frame BEFORE reacting to it and rendering the frame.

	// For reference, below is the debug output of a correct drag operation:
	// [15:51:35] [info]: Frame 62: Drawing model:2 at 183,535
	// [15:51:35] [info]: Frame 63: Drawing model:2 at 183,535
	// [15:51:35] [info]: Frame 64: Drawing model:2 at 183,535
	// [15:51:36] [info]: Frame 65: Drawing model:2 at 183,535
	// [15:51:36] [info]: Frame 65: NODE Drag delta: 8,-3 ENDING?: false
	// [15:51:36] [info]: Drag delta: 8
	// [15:51:36] [info]: Frame 65: Moving model:2 from 183,535 to 191,532, LMB: true, dragging: true, dragEnd: false
	// [15:51:36] [info]: Frame 66: Drawing model:2 at 191,532
	// [15:51:36] [info]: Frame 66: NODE Drag delta: 3,-19 ENDING?: false
	// [15:51:36] [info]: Drag delta: 3
	// [15:51:36] [info]: Frame 66: Moving model:2 from 191,532 to 194,513, LMB: true, dragging: true, dragEnd: false
	// [15:51:36] [info]: Frame 67: Drawing model:2 at 194,513
	// [15:51:36] [info]: Frame 67: NODE Drag delta: -8,-50 ENDING?: false
	// [15:51:36] [info]: Drag delta: -8
	// [15:51:36] [info]: Frame 67: Moving model:2 from 194,513 to 186,463, LMB: true, dragging: true, dragEnd: false
	// [15:51:36] [info]: Frame 68: Drawing model:2 at 186,463
	// [15:51:36] [info]: Frame 68: NODE Drag delta: -11,-88 ENDING?: false
	// [15:51:36] [info]: Drag delta: -11
	// [15:51:36] [info]: Frame 68: Moving model:2 from 186,463 to 175,375, LMB: true, dragging: true, dragEnd: false
	// [15:51:37] [info]: Frame 69: Drawing model:2 at 175,375
	// [15:51:37] [info]: Frame 69: NODE Drag delta: 15,-71 ENDING?: false
	// [15:51:37] [info]: Drag delta: 15
	// [15:51:37] [info]: Frame 69: Moving model:2 from 175,375 to 190,304, LMB: true, dragging: true, dragEnd: false
	// [15:51:37] [info]: Frame 70: Drawing model:2 at 190,304
	// [15:51:37] [info]: Frame 70: NODE Drag delta: 253,-52 ENDING?: false
	// [15:51:37] [info]: Drag delta: 253
	// [15:51:37] [info]: Frame 70: Moving model:2 from 190,304 to 443,252, LMB: true, dragging: true, dragEnd: false
	// [15:51:37] [info]: Frame 71: Drawing model:2 at 443,252
	// [15:51:37] [info]: Frame 71: NODE Drag delta: 98,-6 ENDING?: true
	// [15:51:37] [info]: Frame 72: Drawing model:2 at 443,252
	// [15:51:38] [info]: Frame 73: Drawing model:2 at 443,252
	// [15:51:38] [info]: Frame 74: Drawing model:2 at 443,252
}

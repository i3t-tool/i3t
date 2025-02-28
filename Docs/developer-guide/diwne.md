# DIWNE - Dear Imgui Wrapper Node Editor
Custom implementation of node editor for Dear ImGui library.

## Node editor
Node editor is utility that provide organizing graphical view in graph style. 
Graph consist of nodes (vertices) that can be connected via links (edges). 
Content and graphical representation of nodes are up to you, as well as graphical representation of links.

Nodes are placed in infinity 2D space, and they are movable over this space within window, 
as well as you can move whole space. Nodes can overlap, as well as links.

## Philosophy of DIWNE

DIWNE is "node editor" extension of Dear ImGui library.
Base idea is not implement any additional function (such as rendering or input functions),
but only wrap existing Dear ImGui functions. 
All DIWNE functions provide (just) right order of calling of Dear ImGui functions that leads to node editor behavior.

DIWNE is coded in OOP style and all functions can be overridden. This allows you unlimited customization of view and behavior.

DIWNE does not provide structure where are nodes and links stored. 
You can store them wherever you want as well as it is up to you call "draw" function on those you want to be drawn.

## Base architecture of DIWNE

DIWNE is coded in OOP style and [Template method pattern](https://en.wikipedia.org/wiki/Template_method_pattern). 
That means, that DIWNE function implement right order of calling functions, and you will fill in (override) function 
just for (for example) content of node.

All objects have access to "Diwne (see below)" object, so they can share information via it if needed.
Your objects for node, link, editor etc. will be inherited from DIWNE objects, and so they can 
add any attributes or functionality.

Simplified procedure is override `content()` function of object and call `drawDiwne()` function in place 
in code where you want to draw it.
Functions with "Diwne" in name (`drawDiwne()`, `xxxDiwne()` etc.) are not intended to override (but you can do it).
Be aware what you are doing while overriding those functions.

### OOP structure
- [`DiwneObject`](../cpp-api-reference/classes/classDIWNE_1_1DiwneObject.html) (see `DiwneObject.h` file)

	Class from with all DIWNE objects inherit. Provide base atributes:

	- `diwne`: reference to "node-editor object" that represents whole node-editor.

	Provides sharing inter-object information:

	- `m_idDiwne`, `m_labelDiwne`, `m_popupLabelDiwne`: identification attributes used for creating ImGui id.

	- `m_inner_interaction_happen`, `m_inner_interaction_happen_previous_draw`: interaction information.

	- `m_drawMode`, `m_selectable`, `m_isHeld`, `m_isDragged`, etc.: actual usage information.

- [`Diwne`](../cpp-api-reference/classes/classDIWNE_1_1Diwne.html) (see `diwne.h` file)
	
	Class that represents whole node-editor. Process "workarea" aka "2D space" operation.

	Your main (node-editor) object will inherit from this and then override content() function with all content 
    you want to see. 
    When you want draw node-editor, call drawDiwne() on this object. 

	Provide shared functions and information and workarea information:

	- `mp_settingsDiwne`: store information with setting (colors, margins, sizes etc.).
	- `m_diwneAction`, `m_diwneAction_previousFrame`: actual and previous user action.
	- `mp_lastActivePin`, `mp_lastActiveNode`: last active objects used by user.
	- `m_workAreaScreen`, `m_workAreaDiwne`: what part of workarea is visible on screen. m_workAreaScreen is basically.
      just window, `m_workAreaDiwne` depends on user translation of space (`m_workAreaDiwne.Min`), window size and zoom. 
      See `Diwne::updateWorkAreaRectangles()`.
	- transform coordinates between ImGui-screen-workarea.
	- draw some basic icons.
	- `bypass...()` functions: name "bypass" is confusing a little bit. Wrap original ImGui functions which allows 
      you to change default ImGui behavior by override bypass...() function. 

- [`Node`](../cpp-api-reference/classes/classDIWNE_1_1Node.html) (see `Node.h`)
	
	Object representing node (vertex). Your Node will inherit from this object.
	For draw node call `drawDiwne()` on object

	Node has graphical parts:
   ```
   -------------------
   |      Top        |
   -------------------
   |Left|Middle|Right|    -> |Left|Middle|Right| == Center
   -------------------
   |     Bottom      |
   -------------------
   ```

   For change content of `Node` override `topContent()`, `leftContent()` etc. functions.

  `content()` function is already prepared (see `Node::content()`) so you need not override it (if you do not want).

   Size of node and its parts are updated in the end of node drawing, see `Node::updateSizes()`.

- [`Pin`](../cpp-api-reference/classes/classDIWNE_1_1Pin.html) (see `Pin.h`)

	Object representing point (usually square) from and to which link is connected. 
    Pin will be probably part of your node (can be invisible) and link will lead to it.

	Pull the link from pin is possible by mouse and pin is only possible place from with it is possible (by default).

	Main atributes:

	- `m_connectionPointDiwne`: point to/from which will be link drawn (by default).

- [`Link`](../cpp-api-reference/classes/classDIWNE_1_1Link.html) (see `Link.h` file)

  Object representing link (edge in graph). It is Bézier curve with four parameters - end points and one control 
  point for each end point.


## TODO

- Minimal working example
- Re-name bypass functions
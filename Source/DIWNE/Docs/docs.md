# DIWNE - Dear Imgui Wrapper Node Editor

## Reasons, Background and Story

DIWNE was written as one UI part of I3T. On the other hand, intention was DIWNE as independent generally usable utility/library that is (only) also used in I3T. Graphical part of I3T is based on DearImgui library, therefore DIWNE is based on DearImgui as well.

DearImgui itself has no "node-editor" support. Other libraries based on providing node-editing has some limitations (some hard-coded graphical settings like margins etc., usually there is no support for "nodes inside other nodes" - this funcionality is necessary in I3T, more over other libraries often provide their own rendering function which can easily leads to uncompatibility with newer version of DearImgui)

DIWNE is designed only as wrapper of DearImgui function (call DearImgui orginal function in right order). It provide absolute freedom in graphical setting of any part od node-editor as well as absolute freedom for re-setting actions possible with object in node editor. Default behavior is not hard-coded and is prepared to be (hopefully) easilly changed.

Benefits of DIWNE is high variability and only one dependence (DearImgui).
TODO and improvements are in the end of this document.

## Node editor

Node editor is utitilty that provide organizing grafical view in graph style. Graph consist of nodes (verticies) that can be connected via links (edges). Content and graphical representation of nodes are up to you, as well as graphical representation of links.

Nodes are placed in infinty 2D space and they are movable over this space within window, as well as you can move whole space. Nodes can overlap, as well as links.

## Philosophy of DIWNE

DIWNE is "node editor" extension of DearImgui library. Base idea is not implement any additional function (such as rendering or input functions), but only wrap existing DearImgui functions. All DIWNE functions provide (just) right order of calling of DearImgui functions that leads to node editor behavior.

DIWNE is coded in OOP style and all functions can be override. This allow you ulimited customization of view and behavior.

DIWNE do not provide structure where are nodes and links stored. You can store them wherever you want as well as it is up to you call "draw" function on those you want to be drawn.

## Base architecture of DIWNE

DIWNE is coded in OOP style and [Template method pattern](https://en.wikipedia.org/wiki/Template_method_pattern). That means, that DIWNE function implement right order of calling functions and you will fill in (override) function just for (for example) content of node.

All objects have acces to "Diwne (see below)" object, so they can share information via it if needed.

Your objects for node, link, editor etc. will be inhereted from DIWNE objects and so they can add any atributes or functionality.

Simplified procedure is override content() function of object and call drawDiwne() function in place in code where you want to draw it.

Functions with "Diwne" in name ( drawDiwne(), xxxDiwne() etc. ) are not intended to override (but you can do it). Be aware what you are doing while overriding those functions.

### OOP structure
- DiwneObject (see DiwneObject.h)

	Class from with all DIWNE objects inherit. Provide base atributes:

	- diwne : reference to "node-editor object" that represents whole node-editor

	Provides sharing inter-object information.

	- m_idDiwne, m_labelDiwne, m_popupLabelDiwne : identification atributes used for creating ImGui id

	- m_inner_interaction_happen, m_inner_interaction_happen_previous_draw : interaction information 

	- m_drawMode, m_selectable, m_isHeld, m_isDragged, etc. : actual usage information

- Diwne (see diwne.h)
	
	Class that represents whole node-editor. Process "workarea" aka "2D space" operation.

	Your main (node-editor) object will inherit from this and then override content() function with all content you want to see. When you want draw node-editor, call drawDiwne() on this object. 

	Provide shared functions and informations and workarea information:

	- mp_settingsDiwne : store infomration with setting (colors, margins, sizes etc.)
	- m_diwneAction, m_diwneAction_previousFrame : actual and previous user action
	- mp_lastActivePin, mp_lastActiveNode : last active objects used by user
	- m_workAreaScreen, m_workAreaDiwne : what part of workarea is visible on screen. m_workAreaScreen is basiclly just window, m_workAreaDiwne depends on user translation of space (m_workAreaDiwne.Min), window size and zoom. See Diwne::updateWorkAreaRectangles()
	- transform coordinates between ImGui-screen-workarea
	- draw some basic icons
	- bypass...() functions: name "bypass" is confusing a little bit. Wrap original ImGui functions which allows you to change default ImGui behavior by override bypass...() function. 

- Node (see Node.h)
	
	Object representing node (vertex). Your Node will inherit from this object.

	For draw node call drawDiwne() on object

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

   For change content of Node override topContent(), leftContent() etc. functions.

   content() function is already prepared (see Node::content()) so you need not override it (if you do not want).

   Size of node and its parts are updated in the end of node drawing (see Node::updateSizes())

- Pin (see Pin.h)

	Object representing point (usualy square) from and to which link is connected. Pin will be probably part of your node (can be invisible) and link will lead to it.

	Pull the link from pin is possible by mouse and pin is only place from with it is possible (by default).

	Main atributes:

	- m_connectionPointDiwne : point to/from which will be link drawn (by default)

- Link (see Link.h)

	Object representing link (edge in graph). It is Bezier curve with four parameters - end points and one control point for each end point.



## TODO
Some part of DIWNE are not well programmed and will be great to improve it:
- Minimal working example
- Re-name bypass functions
- Proccesing of user action and inter-object interactions. Nowadays its very messy in different part of code and objects. It seems that good idea is only store information, that some action/interaction happens in particular object and leave reaction to this (and all other inter/actions that can happen in one frame) inter/action to the ond of frame-processing. With this aproach could be possible to create clear and easy to check (action to action) matrix. With this matrix will be possible easilly solve situation where two actions interfer. It violate ImGui philosophy, but in complicated (a lot of mutualy interacting objects) is very messy to react immediately after action in every single object, because this action can interfer with previous and/or subsequent events. 
- Zooming of node-editor is now solved by increasing size on font and icons (and therfore whole nodes). It is not well worked aproach because it could affect other parts of application too and some unpredictible consequencies can happen. Better aproach provide for example https://github.com/thedmd/imgui-node-editor but resources demands are not known.  

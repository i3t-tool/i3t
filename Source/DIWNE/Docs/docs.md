# DIWNE - Dear Imgui Wrapper Node Editor

## Reasons, Background and Story

DIWNE was written as one part of the I3T user interface. On the other hand, DIWNE was intended as an independent general-purpose tool/library that is (only) used in I3T. The graphical part of I3T is based on the DearImgui library, so DIWNE is also based on DearImgui.

DearImgui itself has no support for a "node-editor". Other libraries based on providing node-editing have some limitations (some fixed graphical settings like borders etc., and usually no support for "nodes inside other nodes" - this functionality is necessary in I3T, plus other libraries often provide their own rendering functions, which can easily lead to incompatibility with newer versions of DearImgui).

DIWNE is designed only as a wrapper function of DearImgui (calling the original DearImgui functions in the correct order). It provides absolute freedom in the graphical setup of any part of the node-editor, as well as absolute freedom to override the actions possible with an object in the node-editor. The default behavior is not hard-coded and is ready to be (hopefully) easily changed.

DIWNE has the advantage of high variability and only one dependency (DearImgui).
TODOs and enhancements are described at the end of this document.

## Node editor

The node editor is a tool that allows you to arrange a graphical view in a graph style. A graph consists of nodes (vertices) that can be connected by links (edges). The content and graphical representation of the nodes is up to you, as is the graphical representation of the links.

The nodes are placed in an infinite 2D space and can be moved around within the window, as well as the entire space. Nodes can overlap, as well as links.

## Philosophy of the DIWNE Library

DIWNE is a "node editor", an extension of the DearImgui library. The basic idea is not to implement any additional features (such as rendering or input functions), but only to wrap existing DearImgui features. All DIWNE functions provide (just) the correct order of calling DearImgui functions, which leads to the behavior of the node editor.

DIWNE is coded in OOP style and all functions can be overridden. This allows unlimited customization of the view and behavior.

DIWNE does not provide a structure for storing nodes and links. You can store them where you want, just as it is up to you to call the "draw" function on the ones you want to draw.

## Basic architecture of DIWNE

DIWNE is coded in the OOP style and [Template method pattern](https://en.wikipedia.org/wiki/Template_method_pattern). This means that DIWNE functions implement the correct order of function calls and you add (override), for example, a function just for the contents of a node.

All objects have access to the "Diwne object (see below)", so they can share information through it if needed.

Your objects for node, link, editor, etc. will be derived from the DIWNE objects, and can add any attributes or functions.

A simplified approach is to override the content() function of the object and call drawDiwne() at the place in the code where you want to render it.

Functions with "Diwne" in the name ( drawDiwne(), xxxDiwne(), etc. ) are not meant to be overridden (but you can still do it). When overriding these functions, be aware of what you are doing.

### OOP structure
- DiwneObject (see DiwneObject.h)

  The class from which all DIWNE objects inherit. Provides basic attributes:

	- diwne : a reference to a "node-editor object" that represents the entire node-editor.

  Provides information sharing between objects.

	- m_idDiwne, m_labelDiwne, m_popupLabelDiwne : identification attributes used to create the ImGui ID.

	- m_inner_interaction_happen, m_inner_interaction_happen_previous_draw : interaction information 

	- m_drawMode, m_selectable, m_isHeld, m_isDragged, etc. : current  usage information




- Diwne (see diwne.h)

   	A class that represents the entire node editor. It handles the operation "workarea" aka "2D space".

	Your main object (node-editor) will inherit from this class and then override the content() function with all the content you want to display. When you want to render the node-editor, call the drawDiwne() function on this object. 

	PIt provides shared functions and information and workspace information:
	- mp_settingsDiwne : stores setting information (colors, margins, sizes, etc.)
	- m_diwneAction, m_diwneAction_previousFrame : current and previous user actions
	- mp_lastActivePin, mp_lastActiveNode : last active objects that the user recently used
	- m_workAreaScreen, m_workAreaDiwne : what part of the workspace is visible on the screen. The m_workAreaScreen is basically just a window, m_workAreaDiwne depends on the user's translation of the space (m_workAreaDiwne.Min), the window size and the zoom. See Diwne::updateWorkAreaRectangles()
	- transform coordinates between ImGui-screen-workareas
	- draw some basic icons
	- Bypass...() function: the name "bypass" is a bit confusing. It wraps the original ImGui functions, allowing you to change the default ImGui behavior by overriding it. 

- Node (see Node.h)
	
	The object representing the node (vertex). Your node object will inherit from this Node object.

	To draw a node, call the drawDiwne() function on this object.

	A node has graphical parts:
   ```
   -------------------
   |      Top        |
   -------------------
   |Left|Middle|Right|    -> |Left|Middle|Right| == Center
   -------------------
   |     Bottom      |
   -------------------
   ```
   
   To change the content of the node, define the functions topContent(), leftContent() etc..

   The content() function is already prepared (see Node::content()), so you don't need to override it (if you don't want to).

   The size of the node and its parts is updated at the end of the node drawing (see Node::updateSizes()).

 

- Pin (see Pin.h)

	An object representing a point (usually a square) from which and to which the links are connected. The pin is likely to be part of your node (it may be invisible) and the link will lead to it.

	You can pull the link from the pin with the mouse, and the pin is the only place from which this is possible (by default).

	Main attributes:

	- m_connectionPointDiwne : the point to/from which the link will be pulled (by default).
	
- Link (see Link.h)

	An object representing a connection (an edge in the graph). It is a Bezier curve with four parameters - two endpoints and one control point for each endpoint.

## TODO
Some parts of DIWNE are not well programmed and it would be great to improve them:
- A minimal example of how to work with the library
- Renaming the bypass() functions
- Handling user actions and interactions between objects. 

There is currently a big mess in various parts of the code and objects. It seems like a good idea to just store the information that an action/interaction is taking place in a given object, and leave the response to that (and any other interactions that may take place in a single frame) interaction/action for the end of the frame processing.  

With this approach, it would be possible to create a clear and easily auditable (action to action) matrix. Using this matrix, it would be easy to deal with situations where two actions interfere. This goes against the ImGui philosophy, but in a complicated environment (many interacting objects) it is very complicated to react immediately after an action in each individual object, as this action may interfere with previous and/or subsequent events. 
- The zooming of the node editor is now solved by increasing the size of the font and icons (and thus the whole nodes). This is not a well thought out approach as it could affect other parts of the application and could have unpredictable consequences. A better approach is offered by https://github.com/thedmd/imgui-node-editor, for example, but the resource requirements are unknown.    

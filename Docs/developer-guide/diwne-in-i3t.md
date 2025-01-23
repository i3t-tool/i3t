# Usage of DIWNE in I3T
I3T uses the Immediate Mode Graphical User Interface method to create its user interface. 

Immediate Mode Graphical User Interface, also known as IMGUI, is a type of graphical user interface often used 
for developing games or interactive applications. IMGUI differs from traditional graphical user interfaces in that 
it consists of individual elements that are always recreated on the screen with each frame, without being 
stored in memory.

The particular library used for the development of I3T is DearIMGUI [Cor22]. 

For the 2D Workspace where the use constructs s scene graph using a node editor, we developed 
DIWNE - Dear Imgui Wrapper Node Editor library. 
DIWNE is a generic library. In I3T, we used its basic functions as well as some specially adapted for I3T

## WorkspaceDiwne (see WorkspaceDiwne.h)

A class that represents the entire node editor space. It is inherited from `DIWNE::Diwne`. 
Implements node operations such as `selectAll()`, `invertSelection()`, `zoomToAll()`, etc.

In I3T, there is only one instance of it - `g_workspaceDiwne`, because only one node editor is used.

Important attributes:
- `ImDrawListSplitter m_channelSplitter`: is used to draw each node into a separate ImGui channel - the nodes are 
  then rendered in the reverse order of their appearance on the screen (the highest node is drawn first),
  as it is necessary to process the user action in order from the highest node to the lowest, and to block this action 
  for lower nodes if some other node overlays the lower nodes.
- `m_workspaceCoreNodes`: `std::vector` of all nodes in the node editor except those inside another node
  (Transformation matrix in a Sequence node). These internal nodes are stored in the node they belong to.

## Elements of the node editor

The node editor view comprises several types of objects (Nodes, Pins, and Links). 

For example, I3T has many specific nodes but often share common attributes and functions. 
To avoid code repetition, node classes are decomposed into several "layers" of inheritance. 
You can let your node use a higher-level implementation or override any function you want.

Operations such as deleting from the node editor are done in a "marking way" - the node marks itself as having been 
deleted by the user, and only in the next frame is it actually deleted by the node editor.

### Workspace elements (see WorkspaceElements.h)

Workspace elements implement attributes common to all graphic item objects that are already drawable 
(they implement all necessary functions):

- `PinKind`: all pins in I3T are either input or output.
- `WorkspaceLevelOfDetail`: each I3T node can be scaled to multiple "levels of detail" views 
  (from a thumbnail to a full view)
- `FloatPopupMode`: float values in the popup menu can represent either an angle or a value.
- `numberOfCharWithDecimalPoint()`: this function calculates the number of characters to be drawn for a float type 
  value with the set number of visible digits after the decimal point.

#### Elements

- `WorkspaceNode`: inherits from `DIWNE::Node` and stores the topLabel and middleLabel as the default contents 
  of the top and middle parts of the node.

- `beforeContent()`: fills the background with the whole node (in ImGui, if the drawing order is essential, 
  or the Splitter must be used)
	
- `bypassFocusForInteractionAction()`: defines the area in which the node as a whole can be interacted with 
  (internal elements such as float input are interactable by themselves independently of this). 
  This is currently the top part of the node (the header)
	
- WorkspacePin: inherits from `DIWNE::Pin`, stores the pin label and the choice of whether to display the label.

### Workspace elements with core data (see WorkspaceElementsWithCoreData.h)

Implementation of elements interconnected with the I3T computational core. The core has its own node representation 
(called `nodebase`), which has no graphical representation; it just stores values and provides calculations over them. 
Workspace elements with kernel data add graphic values and provide a rendering of the nodes themselves 
in the node editor workspace via DIWNE.

- `WorkspaceNodeWithCoreData`:
	- store the `m_nodebase` pointer to its `Core::Node` counterpart. 
	- stores the graphical settings of a specific node (number of visible digits, level of display detail, 
      data map with information on which values are scalable, which are frozen, 
      and which are tied together by synergies, etc.)
	- fill in some basic menus (precision settings, duplication menu, etc.)

- `WorkspaceNodeWithCoreDataWithPins`: stores two `std::vectors` of input and output pins. 
   Draw input/output pins based on the `leftContent()`/`rightContent()` functions

  `WorkspaceNodeWithCoreDataWithPins::finalize()` POSSIBLE DUPLICATION OF FUNCTION WITH 
  `WorkspaceCoreInputPin.drawDiwne()`: when finalizing this node, the links (stored in the input pins of this node) 
  are added to the list (in WorkspaceDiwne) to be rendered at the end of the frame 
  in the correct channel - for correct overlap with other nodes.


- `WorkspaceCoreLink`: inherits from `DIWNE::Link`, stores pointers to the two pins it links - `WorkspaceCoreOutputPin`
  and `WorkspaceCoreInputPin`. The link itself is stored on the input pin in I3T because, in I3T, 
  only one connection can lead to each input pin - so this input pin is an excellent place to store the link.

- `WorkspaceCorePin`: stores the references to `Core::Pin` and `WorkspaceNodeWithCoreData` to which it belongs.

	- `WorkspaceCoreInputPin`: store WorkspaceCoreLink, because in I3T, only one link can lead 
      to the input pin – therefore, the input pin is an excellent place to store the link.

	  `processCreateAndPlugConstrutorNode()`: a function that creates a new node connected to this pin 
      (based on the type of this pin). It just sets the `WorkspaceDiwneAction` to `CreateAndPlugTypeConstructor`, 
      and the new node is created and connected using the node editor (`WorkspaceDiwne`) at the end of the frame.

	- `WorkspaceCoreOutputPin` and `WorkspaceCoreOutputPinWithData`: sometimes, in the I3T core, the data is stored 
      in the output pin. These classes render this data as part of the output pin. 
      Because of the different data types (and manipulation of those types), output pins are divided into several types 
      (`WorkspaceCoreOutputPinVector3`, `WorkspaceCoreOutputPinVector4`, etc.), and each implements the `drawData()`
      function based on its data type. 

Static function for drawing data (`drawDataVec4`, `drawDataVec3`, `drawDataMat4x4`, etc.) composed of floats 
able to invoke popup menu (if the float is editable according to the datamap).

### Transformation (see `WorkspaceTransformation.h` file)

Implementation of common attributes and functions for transformations, such as information about the sequence 
into which the `Transformation` is inserted, testing whether the `Transformation` is valid 
(respects the conditions of a particular transformation), etc.
	
#### Transformations (see `WorkspaceTransformation_s.h` file)

Implementing the specific type behavior (`Core::ETransformType`). The implementation is in the template style.

### Sequence (see `WorkspaceSequence.h` file)

Provides functions for inserting and removing a transformation to/from a sequence and moving a transformation 
between a sequence and the node editor (workspace). When moving a transformation into a sequence, 
select the transformation to be removed from the workspace and add it to the sequence and vice versa.

### Other node types (`WorkspaceCamera.h`, `WorkspaceCycle.h`, ...)

Inherit from `WorkspaceNodeWithCoreDataWithPins` (or `WorkspaceNodeWithCoreData`) and implement/override functions 
that are specific to this node.
Hopefully, nothing tricky is happening here, so a brief inspection of the code should be enough to figure it out.

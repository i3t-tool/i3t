# Architecture

A brief overview of the I3T application architecture, including the project structure and key components.

## Application Components Overview

The source code of the I3T application is organized into directories according to specific functionalities 
and areas of focus. This section provides a brief summary of the core components. 

```
Data/                       All Assets (fonts, models, scenes, tutorials, ...)
Dependencies/               Directory with external dependencies.
Docs/
Source/
    - Commands/             Commands for the command pattern.
    - Core/                 Core of the application.
    - GUI/                  ImGui windows, dialogs, elements.
        - Elements
            - Dialogs/      
            - Modals/
            - Nodes/       
            - Windows/      
        - ImGui/            Implementation of ImGui backend.
Test/                       Unit tests, same structure as Source/ directory.
```


### Core
The `Core` directory forms the foundation of the application, containing data structures for "boxes" (nodes), 
their connections, and interface definitions for application modules (`Module` class). 
The `Application` class manages the lifecycle, OpenGL context, command processing, and the game loop. 

Key subdirectories include:
- **Input**: Handles user input and maps key combinations to actions.
- **Nodes**: Implements the data model for the node editor, managing operators, transformations, and lifecycle control
  via the `GraphManager` class.
- **Resources**: Manages application resources (textures, models) using the `pgr-framework` for graphics programming.

### DIWNE (Dear ImGui Wrapper for Node Editor)
DIWNE is a custom library for creating a node editor using Dear ImGui, designed to support I3T’s requirements. 
It abstracts data representation to allow flexibility in integrating various data structures.
DIWNE powers the interactive content within the `WorkspaceWindow`.


### GUI
The `GUI` directory contains code for the graphical user interface:

- **UIModule**: Creates dockable windows and manages styling.
- **Elements**: Implements all UI elements and dialogs.
- **Theme**: Defines and stores style configurations (colors, sizes, fonts).
- **Workspace**: Implements the node editor’s graphical interface, connecting to its core data structures.


### State
This module implements the Memento design pattern for saving and restoring application states. 
It supports undo/redo functionality and saves scenes and settings to files for later restoration.


### Tutorial
Defines the data structure for tutorials, handling file loading and rendering. The `TutorialWindow` class 
displays tutorial content.


### Viewport
Manages scene rendering using OpenGL, including object selection and outlining selected objects.


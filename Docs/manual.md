# I3T Manual

Introduction and a short program description

## User interface

Description of main windows (start window, workspace, scene view), top bar (how to load, append, save...), shortcuts

### Windows
- Start window - contains tutorials that teach basic controls
- Workspace - main window for working with the program
- Scene view - shows what you created in the workspace window
- Tutorial window - shows tutorials

### Main bars
File - ...
Edit - ...
Windows - ...
Tutorials - ...
Help - ...

### Shortcuts
copy/paste/cut, ...

## Workspace

Deeper description of the node system - how to add nodes, connect them and work with them

### Tracking

Introduce the tracking functionality, define what is needed to run it and showcase the controls (smooth/jagged tracking)

### Camera

Introduce cameras and screens and showcase how they work in the program

### Operators

Define the use cases for operators and show a sample situation, show the 10th folta scene

## Manipulators

Introduce manipulators

## Tutorials

In I3T, tutorials are an interactive way for new users to become more familiar with the program while learning important theory about I3T features.

A total of **nine tutorials** are available. **Five** of them focus on the **basics**, while the remaining **four** focus more on **advanced features**.

Before working with the program, we recommend to **try to go through at least the basics** (5 introductory tutorials), but if you already know the basics of transformations,
matrix multiplication, projection matrices, and cameras, you may want to take the **alternative tutorial**, which skips the theoretical parts of the introductory series.

If the user already knows what they are doing, they can try **exercises** (the last two tutorials), that test the user's ability to handle complex tasks with more vague instructions.

# Technical manual

## GUI
## DIWNE
## Tutorials

Main classes for handling tutorials are ``TutorialWindow``, ``StartWindow``, ``TutorialLoader``, ``Tutorial``, an ``Theme``

Tutorials can be accessed through the Start window, which lists all tutorials located in the ``Data/tutorials`` folder.

Tutorials are loaded from ``*.tut'' files using the ``TutorialLoader'' class, which processes all data from the source files into an instance of the ``Tutorial'' class. This instance will contain data in a format that can be loaded into the ``TutorialWindow'' class to be rendered and displayed to the user. 

The ``*.tut`` source files are searched for keywords and symbols that alter the way the text is rendered, similar to the Markdown language. Appearance and form of elements created using keywords can be updated in the ``TutorialWindow`` class in the ``renderHint()`` (or ``render*Something*()``) functions. 

Styling, fonts and colors of the window are stored in the ``Theme.cpp`` and ``Theme.h``, which can be modified to style the tutorial window and its contents.

If you need to create new tutorials, follow [these instructions](tutorials.md).

## etc...

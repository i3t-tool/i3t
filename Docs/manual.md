# I3T Manual

Introduction and a short program description. A good idea is to read the tutorial (In Czech: 1. Začínáme s I3T a 5. Pokročilé funkce programu).

## User interface
![startWindow](./assets/startWindow.png?raw=true) 
The program starts with a *Start window* that serves as a signpost between selecting ready-made tutorials, opening an existing scene, or starting a new one.

Once selected, it enters the workspace mode, consisting of two main windows. ![i3tscreens](../Data/Tutorials/1TUT/i3tscreens.png?raw=true)

In the *workspace* window, the user prepares individual transformations and models and plugs
them into the scene graph. The resulting 3D representation of the structure generated
by the graph is immediately visible in the 3D view in the *Scene View* window. 

The basic building block in the workspace window is a small rectangle
with the title and contents, called *box*. 
![Matrix with a model](../Data/Tutorials/1TUT/hintPosun.png?raw=true)
The matrices are
boxes with a 4x4 grid of real numbers. Matrices affect the object
by being combined in the scene graph as they are put into a
larger box called a sequence.

Sequences have inputs and outputs marked by small icons.
The ![x icon](../Data/Tutorials/1TUT/mult.png?raw=true) 
represents matrix multiplication and matrices are multiplied
by using the left-to-right order. The remaining ![icons with triangles](../Data/Tutorials/1TUT/vec3.png?raw=true) represent
either input or output of the copy of the box.

The key benefit of the I3T tool is its interactivity. The user can
construct the scene graph interactively by ordering the matrices into
sequences, the sequences into graphs, and also interactively modify
the values in the matrices. The effect of the transformation is
immediately shown in the 3D scene view (Figure 1 and the accompanying
video).


 top bar (how to load, append, save...), shortcuts

### Windows list
- Start window - sign post, contains a list tutorials that teach basic controls
- Workspace - main window for working with the program and buildidng the scene graph
- Scene view - shows what you created in the workspace window

### Main bars
The I3T application has a stadard main menu in its top. 

![Main menu](../Data/Tutorials/ADD_TUT/lista.png?raw=true)

- File 
  - Open - standard file dialog for scene file opening. The scenes should be located in Data/Scenes/ directory.
  - Recent - list of recently used scenes for fast opening
  - Save, Save As - sceen file output to disk
  - Manage Models - Place for including new models into I3T. The format should be .gltf with .bin, or .glfb
  - Exit - leaves I3T
Edit - A small menu with a single item
  - Preferences 
     - setup of the model preview in the model box in the workspace
     - setup of the selected model highliht in the Scene View 
     - setupo of  the grid in the Scene View 
Windows - window selector
Tutorials - Link to start window with the list of tutorials available 
Help 
  - About window with info about the application
  - Demo window of the DearImGui library
  - Style editor for Theme (skin) selection and skin modification 
  - Debug info

### Shortcuts

| **Action**                      | **Shortcut**     |
|---------------------------------|------------------|
| **Copy Component**              | `Ctrl + C`       |
| **Paste Component**             | `Ctrl + V`       |
| **Duplicate Component**         | `Ctrl + D`       |
| **Cut Component**               | `Ctrl + X`       |
| **Select All Components**       | `Ctrl + A`       |
| **Invert Selection**            | `Ctrl + I`       |
| **Undo Movement**               | `Ctrl + Z`       |
| **Redo Movement**               | `Ctrl + Y`       |
| **Zoom To All Components**      | `Ctrl + Alt + A` |
| **Zoom To Selected Components** | `Ctrl + Alt + S` |
| **Save Scene**                  | `Ctrl + S`       |
| **Open Scene**                  | `Ctrl + O`       |


## Workplace

The **Workspace** window is where scenes are assembled. You can add, connect, and manipulate blocks to create the desired transformations and models.

### Adding Blocks

- To add blocks, right-click anywhere in the **Workspace** window to open the "Workspace menu."
- Main block types include:
  - **Transformation**: For moving, rotating, scaling, and projections (e.g., `translate`, `rotate`, `scale`, `projection`).
  - **Operator**: Performs operations on matrices, vectors, quaternions, and floats (e.g., `normalize vec3`, `matrix inversion`, `quat * quat`).
  - **Sequence**: Groups transformations and defines their order.
  - **Model**: Represents 3D objects like cubes.
  - **Camera**: Adds cameras for custom scene views.
  - **Other Blocks**: Includes **Pulse**, **Screen**, and **Cycle** for specific use cases.

### Connecting Blocks

Blocks function like functions, with inputs and outputs. Connect blocks using **wires** to form a scene graph:
- **Matrix input/output (🟥)**: For connecting matrices.
- **Matrix multiplication (❎)**: Combines matrices by multiplying them.
- **3D vector input/output (🔷)**: For connecting vector values.
- **4D vector input/output (🟫)**: For connecting 4D vector values.
- **Float input/output (🟩)**: For connecting float values.
- **Screen output (🟦)**: Used specifically for cameras to output what they "see."
- **Quaternion input/output (🟨)**: For connecting quaternion data.
- **Pulse input/output (🟪)**: For triggering cyclic or time-based operations.

Connections are visualized as colored wires, representing the flow of data through the scene graph. Each type of connection is specific to its data type (e.g., matrices, vectors, floats, quaternions, or pulses).

### Transformations

Transformations are always inserted into sequences in the order **Translation, Rotation, Scaling** (TRS). 

#### Modifying Values

- Values in matrices can be adjusted by:
  - Dragging with the left mouse button.
  - Double-clicking and typing the value.
- Use the **context menu** (right-click on the matrix) to:
  - Disable synergies for independent axis scaling.
  - Access "Choose value" to select specific values from a list (e.g., `1`, `sqrt(2)/2`).

### Tracking

The **Tracking** feature allows you to visualize the effects of transformations applied in sequences:
- Add a tracking copy of the model connected to a sequence.
- Use the arrows to step through transformations and observe how they are applied.
![Tracking show off](../Data/Tutorials/3TUT/tracking.png?raw=true)

### Camera

The **Camera block** allows you to add a camera to your scene. It contains two sequences:
- **Projection**: A container for projection matrices (e.g., orthogonal or perspective).
- **View**: A container for the view matrix, which positions and orients the camera.

The **Camera** block has three outputs:
![Tracking show off](../Data/Tutorials/4TUT/camera_block.png?raw=true)
- 🔷 **Screen Output**: Displays what the camera sees (connect to a `Screen` block).
- 🔴 **Matrix Copy Output**: Outputs the resulting matrix for further manipulation.
- ❎ **Multiplication Output**: Allows multiplication with other matrices.

To visualize what the camera sees:
1. Add a **Screen** block to the workspace.
2. Connect the camera’s **Screen Output** (🔷) to the Screen block.
3. Adjust the screen size by dragging its red corners.

<details> <summary>Projection Matrices</summary>

Projection defines how 3D objects are displayed in 2D. There are two main types:
- **Orthogonal Projection**: Creates a flat view (e.g., blueprints). Object size does not change with distance.
  - Adjust parameters such as `left`, `right`, `top`, `bottom`, `near`, and `far`.
  - Use **disable synergies** to adjust bounds independently.

- **Perspective Projection**: Mimics human vision, where objects appear smaller as they move farther away.
</details>

<details> <summary> View Matrix </summary>

The **View Matrix** defines the camera's position and orientation using the **lookAt** matrix. It consists of:
- **Eye vector**: Camera position.
- **Center vector**: Point the camera looks at.
- **Up vector**: Direction considered "up" for the camera.

The **lookAt** matrix cannot be edited directly but is controlled through these vectors.
</details>

### Cycle

The **Cycle block** facilitates cyclic or time-based operations, providing control over value transitions, looping behaviors, and timing. It enables the creation of animations, repetitive movements, and smooth transitions within your scene.

#### Inputs and Outputs

**Inputs**:
- 🟩 **Float Inputs**:
  - **From**: Starting value of the cycle.
  - **To**: Ending value of the cycle.
  - **Step**: The increment or decrement applied to the value at each step.
- 🟪 **Pulse Inputs**:
  - **Start**: Begins or resumes the cycle.
  - **Pause**: Temporarily halts the cycle without resetting values.
  - **Stop**: Ends the cycle and resets the value to **From**.
  - **Skip to Start**: Immediately sets the value to **From** and resumes if running.
  - **Skip to End**: Immediately sets the value to **To** and resumes if running.

**Outputs**:
- 🟩 **Float Output**:
  - **Value**: The current value of the cycle.
- 🟪 **Pulse Outputs**:
  - Sent at specific events:
    - **Start**: When the cycle begins.
    - **Pause**: When the cycle is paused.
    - **Stop**: When the cycle is stopped and reset.
    - **Skip to Start**: When the value is set to **From**.
    - **Skip to End**: When the value is set to **To**.
    - **Value Increment**: When the value starts increasing.
    - **Value Decrement**: When the value starts decreasing.

#### Parameters

- **From**: Starting value of the cycle.
- **To**: Ending value of the cycle.
- **Step**: Determines the increment or decrement applied to the value at each step.
- **Smooth Step**: Enables smooth transitions throughout the **Step Duration**.
- **Step Duration**: Time (in seconds) for each step.
- **Single Step**: Triggers one step manually, moving the value up or down once.

#### Playback Modes

The **Cycle block** offers three playback modes:
1. **Once**: The cycle runs from **From** to **To** in steps and stops.
2. **Repeat**: The cycle loops, restarting at **From** after reaching **To**.
3. **Ping-Pong**: The cycle alternates direction, moving from **From** to **To**, then back to **From**, and so on.

#### Controls

- **Start/Pause**: Toggles between starting and pausing the cycle.
- **Stop**: Ends the cycle and resets the value to **From**.
- **Skip to Start**: Immediately sets the value to **From**. If the cycle is running, it continues.
- **Skip to End**: Immediately sets the value to **To**. If the cycle is running, it continues.

### Operators

Operators in I3T perform calculations and manipulations on various data types, such as matrices, vectors, quaternions, and floats. They are categorized into different groups based on the type of operation they perform. Operators can be added through the **Workspace menu** and connected to other blocks to achieve specific results.

<details>
  <summary>Transformation Operators</summary>

  <details>
    <summary>Translate</summary>
    Used for moving objects along the X, Y, and Z axes.
  </details>
  <details>
    <summary>EulerAngleX</summary>
    Rotates objects around the X-axis using Euler angles.
  </details>
  <details>
    <summary>EulerAngleY</summary>
    Rotates objects around the Y-axis using Euler angles.
  </details>
  <details>
    <summary>EulerAngleZ</summary>
    Rotates objects around the Z-axis using Euler angles.
  </details>
  <details>
    <summary>Rotate</summary>
    Provides generic rotation in 3D space using rotation matrices.
  </details>
  <details>
    <summary>Scale</summary>
    Changes the size of an object along its X, Y, and Z axes.
  </details>
  <details>
    <summary>Ortho</summary>
    Creates an orthogonal projection matrix.
  </details>
  <details>
    <summary>Perspective</summary>
    Generates a perspective projection matrix for 3D rendering.
  </details>
  <details>
    <summary>Frustum</summary>
    Constructs a frustum projection matrix.
  </details>
  <details>
    <summary>LookAt</summary>
    Creates a view matrix based on eye, center, and up vectors.
  </details>
</details>
&nbsp;
<details>
  <summary>Matrix Operators</summary>

  <details>
    <summary>Matrix</summary>
    Basic matrix initialization and operations.
  </details>
  <details>
    <summary>Inversion</summary>
    Computes the inverse of a matrix.
  </details>
  <details>
    <summary>Transpose</summary>
    Flips the rows and columns of a matrix.
  </details>
  <details>
    <summary>Determinant</summary>
    Calculates the determinant of a matrix.
  </details>
  <details>
    <summary>Mat * Mat</summary>
    Multiplies two matrices together.
  </details>
  <details>
    <summary>Mat * Vec4</summary>
    Multiplies a matrix by a 4D vector.
  </details>
  <details>
    <summary>Vec4 * Mat</summary>
    Multiplies a 4D vector by a matrix.
  </details>
  <details>
    <summary>Mat + Mat</summary>
    Adds two matrices together.
  </details>
</details>
&nbsp;
<details>
  <summary>Vec3 Operators</summary>

  <details>
    <summary>Vec3</summary>
    Creates or manipulates a 3D vector.
  </details>
  <details>
    <summary>Show Vec3</summary>
    Displays the components of a 3D vector.
  </details>
  <details>
    <summary>Vec3 x Vec3</summary>
    Computes the cross product of two 3D vectors.
  </details>
  <details>
    <summary>Vec3 · Vec3</summary>
    Calculates the dot product of two 3D vectors.
  </details>
  <details>
    <summary>Vec3 + Vec3</summary>
    Adds two 3D vectors together.
  </details>
  <details>
    <summary>Vec3 - Vec3</summary>
    Subtracts one 3D vector from another.
  </details>
  <details>
    <summary>Float * Vec3</summary>
    Multiplies a 3D vector by a scalar value.
  </details>
  <details>
    <summary>Normalize Vec3</summary>
    Converts a 3D vector to unit length.
  </details>
  <details>
    <summary>Length(Vec3)</summary>
    Computes the magnitude of a 3D vector.
  </details>
  <details>
    <summary>Mix Vec3</summary>
    Blends two 3D vectors together.
  </details>
</details>
&nbsp;
<details>
  <summary>Vec4 Operators</summary>

  <details>
    <summary>Vec4</summary>
    Creates or manipulates a 4D vector.
  </details>
  <details>
    <summary>Vec4 · Vec4</summary>
    Calculates the dot product of two 4D vectors.
  </details>
  <details>
    <summary>Vec4 + Vec4</summary>
    Adds two 4D vectors together.
  </details>
  <details>
    <summary>Vec4 - Vec4</summary>
    Subtracts one 4D vector from another.
  </details>
  <details>
    <summary>Float * Vec4</summary>
    Multiplies a 4D vector by a scalar value.
  </details>
  <details>
    <summary>Normalize Vec4</summary>
    Converts a 4D vector to unit length.
  </details>
  <details>
    <summary>Perspective Division</summary>
    Applies perspective division to a 4D vector.
  </details>
  <details>
    <summary>Mix Vec4</summary>
    Blends two 4D vectors together.
  </details>
</details>
&nbsp;
<details>
  <summary>Quaternion Operators</summary>

  <details>
    <summary>Quat</summary>
    Creates or manipulates a quaternion.
  </details>
  <details>
    <summary>Quat(Float, Vec3)</summary>
    Generates a quaternion using a float and a 3D vector.
  </details>
  <details>
    <summary>Quat(Angle, Axis)</summary>
    Constructs a quaternion from an angle and axis.
  </details>
  <details>
    <summary>Quat(Vec3, Vec3)</summary>
    Creates a quaternion between two 3D vectors.
  </details>
  <details>
    <summary>Quat → Float, Vec3</summary>
    Converts a quaternion to a float and a 3D vector.
  </details>
  <details>
    <summary>Quat → Angle, Axis</summary>
    Converts a quaternion to an angle and axis representation.
  </details>
  <details>
    <summary>Float * Quat</summary>
    Multiplies a quaternion by a scalar value.
  </details>
  <details>
    <summary>Quat * Quat</summary>
    Multiplies two quaternions together.
  </details>
  <details>
    <summary>Quat → Euler</summary>
    Converts a quaternion to Euler angles.
  </details>
  <details>
    <summary>Euler → Quat</summary>
    Converts Euler angles to a quaternion.
  </details>
  <details>
    <summary>Slerp</summary>
    Performs spherical linear interpolation between two quaternions.
  </details>
  <details>
    <summary>Long Way Slerp</summary>
    Performs spherical linear interpolation with an inverted path.
  </details>
  <details>
    <summary>Lerp</summary>
    Performs linear interpolation between two quaternions.
  </details>
  <details>
    <summary>Quat Conjugate</summary>
    Computes the conjugate of a quaternion.
  </details>
  <details>
    <summary>QVQ*</summary>
    Performs quaternion-vector multiplication.
  </details>
  <details>
    <summary>Inverse Quat</summary>
    Computes the inverse of a quaternion.
  </details>
  <details>
    <summary>Normalize Quat</summary>
    Converts a quaternion to unit length.
  </details>
  <details>
    <summary>Length(Quat)</summary>
    Computes the magnitude of a quaternion.
  </details>
</details>
&nbsp;
<details>
  <summary>Float Operators</summary>

  <details>
    <summary>Float</summary>
    Creates or manipulates a floating-point value.
  </details>
  <details>
    <summary>Clamp Float</summary>
    Restricts a float to a specified range.
  </details>
  <details>
    <summary>Float * Float</summary>
    Multiplies two floating-point values.
  </details>
  <details>
    <summary>Float / Float</summary>
    Divides one floating-point value by another.
  </details>
  <details>
    <summary>Float + Float</summary>
    Adds two floating-point values.
  </details>
  <details>
    <summary>Float ^ Float</summary>
    Raises one float to the power of another.
  </details>
  <details>
    <summary>Mix Float</summary>
    Blends two floating-point values together.
  </details>
  <details>
    <summary>Sin & Cos</summary>
    Computes the sine and cosine of an angle.
  </details>
  <details>
    <summary>Asin & Acos</summary>
    Computes the arcsine and arccosine of a value.
  </details>
  <details>
    <summary>Signum</summary>
    Determines the sign of a floating-point value.
  </details>
</details>
&nbsp;
<details>
  <summary>Conversion Operators</summary>

  <details>
    <summary>Mat → TR</summary>
    Converts a matrix to translation and rotation components.
  </details>
  <details>
    <summary>TR → Mat</summary>
    Constructs a matrix from translation and rotation components.
  </details>
  <details>
    <summary>Mat → Vec4</summary>
    Converts a matrix to a 4D vector.
  </details>
  <details>
    <summary>Mat → Quat</summary>
    Converts a matrix to a quaternion.
  </details>
  <details>
    <summary>Mat → Floats</summary>
    Converts a matrix to floating-point values.
  </details>
  <details>
    <summary>Vec4 → Mat</summary>
    Converts a 4D vector to a matrix.
  </details>
  <details>
    <summary>Vec4 → Vec3</summary>
    Reduces a 4D vector to a 3D vector.
  </details>
  <details>
    <summary>Vec4 → Floats</summary>
    Converts a 4D vector to floating-point values.
  </details>
  <details>
    <summary>Vec3 → Mat</summary>
    Converts a 3D vector to a matrix.
  </details>
  <details>
    <summary>Vec3 → Vec4</summary>
    Expands a 3D vector to a 4D vector.
  </details>
  <details>
    <summary>Vec3 → Floats</summary>
    Converts a 3D vector to floating-point values.
  </details>
  <details>
    <summary>Quat → Floats</summary>
    Converts a quaternion to floating-point values.
  </details>
  <details>
    <summary>Floats → Mat</summary>
    Creates a matrix from floating-point values.
  </details>
  <details>
    <summary>Floats → Vec4</summary>
    Creates a 4D vector from floating-point values.
  </details>
  <details>
    <summary>Floats → Vec3</summary>
    Creates a 3D vector from floating-point values.
  </details>
  <details>
    <summary>Floats → Quat</summary>
    Creates a quaternion from floating-point values.
  </details>
</details>

## Manipulators

Manipulators are interactive tools visible in the **Scene View** that allow users to adjust transformations (translate, rotate, scale) directly on objects. These manipulators provide a visual and intuitive way to modify object properties without directly editing the transformation matrices.

### Types of Manipulators

1. **Translate Manipulator**
  - Shown as colored arrows (Red: X-axis, Green: Y-axis, Blue: Z-axis).
  - Drag the arrows to move the object along a specific axis.

2. **Rotate Manipulator**
  - Appears as colored circles around the object (Red: X-axis, Green: Y-axis, Blue: Z-axis).
  - Click and drag a circle to rotate the object around the corresponding axis.

3. **Scale Manipulator**
  - Displayed as colored squares (Red: X-axis, Green: Y-axis, Blue: Z-axis).
  - Drag the squares to scale the object along a specific axis.

### Using Manipulators

1. **Enable Manipulators**:
  - Manipulators are automatically displayed for selected models in the **Scene View**.
  - Ensure the associated transformation is active/clicked (e.g., `translate`, `eulerAngleX`, `scale`) in the **Workspace**.

2. **Adjusting Values**:
  - Changes made via manipulators are reflected directly in the connected transformation matrices in the **Workspace** window.
  - Combine manipulator adjustments with precise matrix editing for finer control.

### Combining Manipulations

You can apply multiple transformations in sequence using manipulators. For example:
1. **Translate** the cube to the desired position.
2. **Rotate** it to align with other objects in the scene.
3. **Scale** it to fit proportions.

## Tutorials

In I3T, tutorials are an interactive way for new users to become more familiar with the program while learning important theory about I3T features.

A total of **nine tutorials** are available. **Five** of them focus on the **basics**, while the remaining **four** focus more on **advanced features**.

Before working with the program, we recommend to **try to go through at least the basics** (5 introductory tutorials), but if you already know the basics of transformations,
matrix multiplication, projection matrices, and cameras, you may want to take the **alternative tutorial**, which skips the theoretical parts of the introductory series.

If the user already knows what they are doing, they can try **exercises** (the last two tutorials), that test the user's ability to handle complex tasks with more vague instructions.

---
# Technical manual

## GUI
### Themes
The colors and other size properties of the GUI is determined by the current application *Theme*. There are multiple themes to choose from and can be switched and edited at runtime via the *Style Editor* window. 

One default theme is always loaded called the 'Classic' style. This object is initialized in code in the `Theme::createDefaultClassic()` method.
Other themes are loaded from `.yml` files in the `Data/Themes` directory.

A `Theme` (`GUI/Theme/Theme.h`) object contains key-value pairs of style properties (colors (ImVec4) and sizes (floats, ImVec2)). These style properties overwrite the standard Dear ImGui style properties and specify the visual theme of the application. The style properties are also queried from various parts od the GUI codebase and used locally to style specific components or used outside of Dear ImGui. 

Each additional theme loaded from the `.yml` files using `GUI/Theme/ThemeLoader.h` is derived from the 'Classic' theme and initially contains the properties set by the `Theme::initClassicProperties()` method. These properties are then partially or fully overwritten by the style properties present in the styles `.yml` file.

#### Adding a Theme style property
To add a style property, the following must be done in `Theme.h/.cpp`:
 - Create a style property key in one of the enums depending on the type (EColor, ESize, ESizeVec2).
 - Set a default value for the 'Classic' theme in `initClassicProperties()`.
 - Add a human-readable key name in the `Theme::initNames()` method.
 - (optional) Do something with the style variable value at Theme init in the `Theme::apply()` method. Usually this would be overwriting an existing ImGui style variable with it.

## DIWNE
See the [DIWNE documentation](developer-guide/diwne).

## Tutorials

Main classes for handling tutorials are ``TutorialWindow``, ``StartWindow``, ``TutorialLoader``, ``Tutorial``, an ``Theme``

Tutorials can be accessed through the Start window, which lists all tutorials located in the ``Data/tutorials`` folder.

Tutorials are loaded from ``*.tut'' files using the ``TutorialLoader'' class, which processes all data from the source files into an instance of the ``Tutorial'' class. This instance will contain data in a format that can be loaded into the ``TutorialWindow'' class to be rendered and displayed to the user. 

The ``*.tut`` source files are searched for keywords and symbols that alter the way the text is rendered, similar to the Markdown language. Appearance and form of elements created using keywords can be updated in the ``TutorialWindow`` class in the ``renderHint()`` (or ``render*Something*()``) functions. 

Styling, fonts and colors of the window are stored in the ``Theme.cpp`` and ``Theme.h``, which can be modified to style the tutorial window and its contents.

If you need to create new tutorials, follow [these instructions](tutorials.md).

## etc...

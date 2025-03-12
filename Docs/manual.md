# I3T Manual

This manual serves as a comprehensive guide to using the **I3T** tool, focusing on its key features and functionality. It helps users navigate the application, understand its interface, and master tools for 3D transformations and scene composition. For first-time users we recommend following the introductory tutorials (Začínáme s I3T and Pokročilé funkce programu) for a hands-on start.

## User interface
<div style="text-align: center;">
    <img src="./assets/startWindow.png?raw=true" height="350" alt="">
</div>
<br>

The program begins with the **Start Window**, which allows you to choose between tutorials, opening an existing scene, or creating a new one.
Once selected, it enters the Workspace mode, consisting of two main windows. 

<div style="text-align: center;">
    <img src="./assets/i3tScenes.png?raw=true" height="400" alt="">
</div>
<br>
height="350">
In the *Workspace* window, the user prepares individual transformations and models and plugs
them into the scene graph. The resulting 3D representation of the structure generated
by the graph is immediately visible in the 3D view in the *Scene View* window. 

The basic building blocks in the **Workspace** are small rectangles called **nodes**, which contain titles and content.

<div style="text-align: center;">
    <img src="../Docs/assets/buildingBlocks.png?raw=true" height="150" alt="">
</div>
<br>

Matrices function as nodes with a 4x4 grid of real numbers. Matrices affect objects when you combine them in the scene graph and place them into a larger node called a sequence.

Sequences have inputs and outputs marked by small icons.

The greatest strength of the I3T tool lies in its **interactivity**. Users can:
<ul style="margin-left: 20px;">
  <li>Build scene graphs dynamically by connecting matrices into sequences and sequences into graphs.</li>
  <li>Modify matrix values directly and observe the immediate impact in the <strong>Scene View Window</strong>.</li>
</ul>

### Windows list
- Start Window -  a signpost that contains a list of tutorials teaching basic controls
- Workspace - main window for working with the program and building the scene graph
- Scene view - displays a real-time visualization of created scene from the Workspace window

### Main bars
The I3T application has a standard main menu in its top. 
<div style="text-align: center;">
  <img src="../Docs/assets/topBar.png?raw=true" height="50" alt="">
</div>


#### **File Menu**
- **New**: Creates a new scene.
- **Open**: Opens an existing scene file from the `Data/Scenes/` directory. *(Shortcut: Ctrl + O)*
- **Recent**: Displays a list of recently used scenes for quick access.
- **Save**: Saves the current scene. *(Shortcut: Ctrl + S)*
- **Save As**: Saves the current scene under a new name.
- **Manage Models**: Import new models into I3T. Supported formats include `.gltf` (with `.bin`) and `.glfb`.
- **Exit**: Closes the application.

#### **Edit Menu**
- **Style Editor**: Opens the style editor for customizing the application theme.
- **Preferences**:
  - Configure the model preview in the **Workspace**.
  - Set the highlight color for selected models in the **Scene View**.
  - Adjust the grid settings in the **Scene View**.

#### **Windows Menu**
- **Start Window**: Opens the start menu window.
- **Tutorial Window**: Opens tutorial window with link to start window.
- **Scene View Window**: Toggles visibility of the **Scene View** window.
- **Workspace Window**: Toggles visibility of the **Workspace** window.

#### **Tutorials Menu**
- **Start Window**: Opens the start menu window.
- **Reload**: Reloads the current tutorial.

#### **Help Menu**
- **About**: Displays information about the application, including its version and credits.

### Shortcuts

| **Action**                      | **Shortcut**                                    |
|---------------------------------|-------------------------------------------------|
| **Copy Component**              | <kbd>Ctrl</kbd> + <kbd>C</kbd>                  |
| **Paste Component**             | <kbd>Ctrl</kbd> + <kbd>V</kbd>                  |
| **Duplicate Component**         | <kbd>Ctrl</kbd> + <kbd>D</kbd>                  |
| **Cut Component**               | <kbd>Ctrl</kbd> + <kbd>X</kbd>                  |
| **Select All Components**       | <kbd>Ctrl</kbd> + <kbd>A</kbd>                  |
| **Invert Selection**            | <kbd>Ctrl</kbd> + <kbd>I</kbd>                  |
| **Undo Movement**               | <kbd>Ctrl</kbd> + <kbd>Z</kbd>                  |
| **Redo Movement**               | <kbd>Ctrl</kbd> + <kbd>Y</kbd>                  |
| **Zoom To All Components**      | <kbd>Ctrl</kbd> + <kbd>Alt</kbd> + <kbd>A</kbd> |
| **Zoom To Selected Components** | <kbd>Ctrl</kbd> + <kbd>Alt</kbd> + <kbd>S</kbd> |
| **Save Scene**                  | <kbd>Ctrl</kbd> + <kbd>S</kbd>                  |
| **Open Scene**                  | <kbd>Ctrl</kbd> + <kbd>O</kbd>                  |


## Workspace

The **Workspace** window is where scenes are assembled. You can add, connect, and manipulate nodes to create the desired transformations and models.

### Adding Nodes

<img src="../Docs/assets/addBlocks.png?raw=true" alt="">

- To add node, <em>right-click</em> anywhere in the **Workspace** to open the context menu.
- Main node types include:
  - **Transformation**: For moving, rotating, scaling, and projections (e.g., `translate`, `rotate`, `scale`, `projection`).
  - **Operator**: Performs operations on matrices, vectors, quaternions, and floats (e.g., `normalize vec3`, `matrix inversion`, `quat * quat`).
  - **Sequence**: Groups transformations and defines their order.
  - **Model**: Represents 3D objects like cubes.
  - **Camera**: Adds cameras for custom scene views.
  - **Other Nodes**: Includes **Pulse**, **Screen**, and **Cycle** for specific use cases.

### Selecting Nodes

Selecting nodes in the **Workspace** is a key step for organizing and editing your scene graph. There are two main methods for selecting nodes based on the direction of your selection box:

<ol>
  <li>
    <strong>Partial Selection (Green Box):</strong>
    <ul>
      <li>Click and drag with the <em>left mouse button</em> from <strong>right to left</strong> outside of the nodes.</li>
      <li>A green selection box appears. Nodes only need to be partially covered by the selection box to be selected.</li>
      <li>Release the mouse button.</li>
    </ul>
  </li>
  <li>
    <strong>Full Selection (Blue Box):</strong>
    <ul>
      <li>Click and drag with the <em>left mouse button</em> from <strong>left to right</strong> outside of the nodes.</li>
      <li>A blue selection box appears. You must fully cover nodes with the selection box to select them.</li>
      <li>Release the mouse button.</li>
    </ul>
  </li>
</ol>
Selected nodes are outlined in <strong>orange</strong>.

### Connecting Nodes

Nodes function like functions, with inputs and outputs. Connect nodes using **wires** to form a scene graph:
<div style="margin-left: 20px;">
  <table>
    <thead>
      <tr>
        <th><strong>Icon</strong></th>
        <th><strong>Name</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><img src="../Docs/assets/matrix.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>Matrix input/output</strong></td>
        <td>For connecting matrices.</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/matrixMultiplication.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>Matrix multiplication</strong></td>
        <td>Combines matrices by multiplying them.</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/3Dvector.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>3D vector input/output</strong></td>
        <td>For connecting vector values.</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/4Dvector.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>4D vector input/output</strong></td>
        <td>For connecting 4D vector values.</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/float.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>Float input/output</strong></td>
        <td>For connecting float values.</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/screen.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>Screen output</strong></td>
        <td>Used specifically for cameras to output what they "see."</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/quaternion.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>Quaternion input/output</strong></td>
        <td>For connecting quaternion data.</td>
      </tr>
      <tr>
        <td><img src="../Docs/assets/pulse.png?raw=true" width="20" height="20" alt=""></td>
        <td><strong>Pulse input/output</strong></td>
        <td>For triggering cyclic or time-based operations.</td>
      </tr>
    </tbody>
  </table>
</div>

Colored wires visualize connections, showing the flow of data through the scene graph. Each type of connection is specific to its data type (e.g., matrices, vectors, floats, quaternions, or pulses).

### Transformations

Transformations are always inserted into sequences. The order can be arbitrary. A typical order is **Translation, Rotation, Scaling** (TRS). 

<img src="../Docs/assets/transformations.png?raw=true" height="150" alt="">



#### How to Add Transformations to a Sequence
<img src="../Docs/assets/sequenceGIF.gif?raw=true" height="200" alt="">

<ol>
  <li>
    <strong>Select the Transformation Node:</strong>
    <ul>
      <li>Locate the transformation node in the <em>Workspace Menu</em>.</li>
    </ul>
  </li>
  <li>
    <strong>Drag and Drop:</strong>
    <ul>
      <li>Click and hold the transformation node with the <em>left mouse button</em>.</li>
      <li>Drag the node to the desired position <strong>inside a sequence</strong>.</li>
      <li>Release the mouse button to drop the node into the sequence.</li>
    </ul>
  </li>
  <li>
    <strong>Sequence Multiplication:</strong>
    <ul>
      <li>Transformations in a sequence are <strong>multiplied from left to right</strong>, meaning the order matters.</li>
      <li>For example, placing <code>Translate</code> followed by <code>Rotate</code> will first move the object, then rotate it around the new position.</li>
    </ul>
  </li>
</ol>

#### Modifying Values
<img src="../Docs/assets/modifyingGIF.gif?raw=true" height="200" alt="">

<ul>
  <li>
    <strong>Adjust Values with Dragging:</strong>
    <ul>
      <li>Click and hold the value in the matrix using the <em>left mouse button</em>.</li>
      <li>Drag the mouse to increase or decrease the value.</li>
    </ul>
  </li>
  <li>
    <strong>Edit Values Directly:</strong>
    <ul>
      <li>Double-click on the value in the matrix to open an input box.</li>
      <li>Type the desired value and press <em>Enter</em> to confirm.</li>
    </ul>
  </li>
  <li>
    <strong>Use the Context Menu:</strong>
    <ul>
      <li>Right-click on the matrix to open the context menu.</li>
      <li>
        Options include:
        <ul>
          <li><strong>Disable Synergies:</strong> Adjust scaling independently for each axis.</li>
          <li><strong>Choose Value:</strong> Select specific values like <code>1</code>, <code>sqrt(2)/2</code>, or others from a predefined list.</li>
        </ul>
      </li>
    </ul>
  </li>
</ul>


### Tracking

The **Tracking** feature allows you to observe how transformations are applied step-by-step in a sequence by interpolating matrices over time. This is particularly useful for understanding and debugging transformations.

<img src="../Docs/assets/tracking.png?raw=true" height="120" alt="">

<details>
  <summary style="font-weight: bold; cursor: pointer;">How to Enable Tracking</summary>
  <ol>
    <li>
      <strong>Right-Click on a Sequence Node:</strong>
      <ul>
        <li>In the <em>Workspace</em>, right-click on a sequence node to open the context menu.</li>
        <li>Choose one of the following options:
          <ul>
            <li><strong>Start Tracking from Left:</strong> Tracking begins with the first transformation in the sequence.</li>
            <li><strong>Start Tracking from Right:</strong> Tracking begins with the last transformation in the sequence.</li>
          </ul>
        </li>
      </ul>
    </li>
    <li>
      <strong>Tracking Copy:</strong>
      <ul>
        <li>A copy of the model appears in the <em>Scene View</em>, allowing you to visualize the transformations interactively.</li>
      </ul>
    </li>
  </ol>
</details>

<details>
  <summary style="font-weight: bold; cursor: pointer;">How Tracking Works</summary>
  <p style="margin-left: 20px">
    Tracking helps visualize how transformations are applied in sequences by interpolating between transformation matrices step by step. 
  </p>

  <ul>
    <details>
      <summary style="font-weight: bold; cursor: pointer;">Start Tracking from Left</summary>
      <ul style="margin-left: 20px">
        <li>The tracking line starts at the <strong>leftmost position</strong> of the sequence.</li>
        <li>Each transformation is applied sequentially, gradually altering the coordinate system.</li>
        <li>For example, with a TRS sequence:
        <br>
        <img src="../Docs/assets/trackingFromLeft.gif?raw=true" height="300" alt="">
          <ul style="margin-left: 20px">
            <li>First, the coordinate system is <strong>translated</strong>, moving the reference point.</li>
            <li>Next, it is <strong>rotated</strong>, changing the orientation of subsequent transformations.</li>
            <li>Finally, the object is <strong>scaled</strong> in the transformed coordinate system.</li>
          </ul>
        </li>
        <li>Pressing the <strong>right arrow key</strong> moves the line forward.</li>
        <li>Shows how transformations progressively modify the coordinate system before applying them to the object.</li>
      </ul>
    </details>
    <details>
      <summary style="font-weight: bold; cursor: pointer;">Start Tracking from Right</summary>
      <ul style="margin-left: 20px">
        <li>The tracking line starts at the <strong>rightmost position</strong> of the sequence.</li>
        <li>Each transformation is applied in reverse order, maintaining a global coordinate system.</li>
        <li>For example, with a TRS sequence:
          <ul style="margin-left: 20px">
            <li>First, the object is <strong>scaled</strong> in global coordinates.</li>
            <li>Next, it is <strong>rotated</strong>, affecting the entire model uniformly.</li>
            <li>Finally, it is <strong>translated</strong>, shifting its final position.</li>
          </ul>
        </li>
        <li>Pressing the <strong>left arrow key</strong> moves the line backward.</li>
        <li>Shows how transformations affect the object directly in global space before altering the coordinate system.</li>
      </ul>
    </details>
  </ul>
</details>


<details>
  <summary style="font-weight: bold; cursor: pointer;">Stopping Tracking</summary>
  <ul>
    <li>
      <strong>Right-Click on the Sequence Node:</strong>
      <ul>
        <li>To stop tracking, right-click on the sequence node again and select <strong>Stop Tracking</strong> from the context menu.</li>
        <li>The tracking copy and line will be removed.</li>
      </ul>
    </li>
  </ul>
</details>


### Camera

The **Camera node** allows you to add a camera to your scene. It contains two sequences:
- **Projection**: A container for projection matrices (e.g., orthogonal or perspective).
- **View**: A container for the view matrix, which positions and orients the camera.

The **Camera** node has three outputs:

<img src="../Docs/assets/camera.png?raw=true" height="100" alt="">

- <img src="../Docs/assets/screen.png?raw=true" width="20" height="20" alt="">  **Screen Output**: Displays what the camera sees (connect to a `Screen` node).
- <img src="../Docs/assets/matrix.png?raw=true" width="20" height="20" alt="">  **Matrix Copy Output**: Outputs the resulting matrix for further manipulation.
- <img src="../Docs/assets/matrixMultiplication.png?raw=true" width="20" height="20" alt="">  **Multiplication Output**: Allows multiplication with other matrices.

To visualize what the camera sees:
1. Add a **Screen** node to the Workspace.
2. Connect the camera’s <img src="../Docs/assets/screen.png?raw=true" width="20" height="20"> **Screen Output** to the Screen node.
3. Adjust the screen size by dragging its red corners.

<details>
  <summary style="font-weight: bold; cursor: pointer;">Projection Matrices</summary>
  <div style="margin-left: 20px;">
    <strong>Orthogonal Projection</strong>
    <ul>
      <li>Creates a flat view (e.g., blueprints), where object size remains constant regardless of distance.</li>
      <li>Use <strong>disable synergies</strong> to adjust bounds independently.</li>
    </ul>
    <strong>Perspective Projection</strong>
    <ul>
      <li>Mimics human vision, where objects appear smaller as they move farther away.</li>
      <li>Commonly used for realistic 3D scenes requiring depth perception.</li>
    </ul>
  </div>
</details>

<details>
  <summary style="font-weight: bold; cursor: pointer;">View Matrix</summary>
  <div style="margin-left: 20px;">
    <strong>The View Matrix</strong> defines the camera's position and orientation using the <strong>lookAt</strong> matrix. It consists of:
    <ul>
      <li><strong>Eye vector:</strong> Camera position.</li>
      <li><strong>Center vector:</strong> Point the camera looks at.</li>
      <li><strong>Up vector:</strong> Direction considered "up" for the camera.</li>
    </ul>
    <p>The <strong>lookAt</strong> matrix cannot be edited directly but is controlled through these vectors.</p>
  </div>
</details>

<p><br>
<strong>Theory: </strong>
<br>
<a href="https://cent.felk.cvut.cz/courses/PGR/lectures/05_Transform_2.pdf" target="_blank" rel="noopener noreferrer">Projection and viewport</a>
<br>
<a href="https://cent.felk.cvut.cz/courses/PGR/lectures/05-transformace-2-priloha.pdf" target="_blank" rel="noopener noreferrer">More on projection transformations</a></p>


### Cycle

The **Cycle node** facilitates cyclic or time-based operations, providing control over value transitions, looping behaviors, and timing. It enables the creation of animations, repetitive movements, and smooth transitions within your scene.

<img src="../Docs/assets/cycle.png?raw=true" height="150" alt="">

#### Inputs and Outputs
<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px;">Inputs</summary>

  <div style="margin-left: 40px;">
    <p><img src="../Docs/assets/float.png?raw=true" width="20" height="20" alt=""><strong> Float Inputs:</strong><br>These inputs control the numerical values for the cycle's progression.</p>
    <table>
      <thead>
        <tr>
          <th><strong>Input</strong></th>
          <th><strong>Description</strong></th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td><strong>From</strong></td>
          <td>Starting value of the cycle.</td>
        </tr>
        <tr>
          <td><strong>To</strong></td>
          <td>Ending value of the cycle.</td>
        </tr>
        <tr>
          <td><strong>Step</strong></td>
          <td>The increment or decrement applied at each step.</td>
        </tr>
      </tbody>
    </table>
    &nbsp;
    <p><img src="../Docs/assets/pulse.png?raw=true" width="20" height="20" alt=""><strong> Pulse Inputs:</strong><br>These inputs trigger actions or changes within the cycle.</p>
    <table>
      <thead>
        <tr>
          <th><strong>Input</strong></th>
          <th><strong>Description</strong></th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td><strong>Start</strong></td>
          <td>Begins or resumes the cycle.</td>
        </tr>
        <tr>
          <td><strong>Pause</strong></td>
          <td>Temporarily halts the cycle without resetting values.</td>
        </tr>
        <tr>
          <td><strong>Stop</strong></td>
          <td>Ends the cycle and resets the value to <strong>From</strong>.</td>
        </tr>
        <tr>
          <td><strong>Skip to Start</strong></td>
          <td>Sets the cycle to <strong>From</strong> and continues if running.</td>
        </tr>
        <tr>
          <td><strong>Skip to End</strong></td>
          <td>Sets the cycle to <strong>To</strong> and continues if running.</td>
        </tr>
      </tbody>
    </table>
  </div>
</details>
&nbsp;
<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px;">Outputs</summary>

  <div style="margin-left: 40px;">
    <p><img src="../Docs/assets/float.png?raw=true" width="20" height="20" alt=""><strong> Float Output:</strong><br>This output provides the current value of the cycle.</p>
    <table>
      <thead>
        <tr>
          <th><strong>Output</strong></th>
          <th><strong>Description</strong></th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td><strong>Value</strong></td>
          <td>The current value of the cycle.</td>
        </tr>
      </tbody>
    </table>
    &nbsp;
    <p><img src="../Docs/assets/pulse.png?raw=true" width="20" height="20" alt=""><strong> Pulse Outputs:</strong><br>These outputs are triggered during specific cycle events.</p>
    <table>
      <thead>
        <tr>
          <th><strong>Output</strong></th>
          <th><strong>Description</strong></th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td><strong>Start</strong></td>
          <td>Triggered when the cycle begins.</td>
        </tr>
        <tr>
          <td><strong>Pause</strong></td>
          <td>Triggered when the cycle is paused.</td>
        </tr>
        <tr>
          <td><strong>Stop</strong></td>
          <td>Triggered when the cycle is stopped and reset.</td>
        </tr>
        <tr>
          <td><strong>Skip to Start</strong></td>
          <td>Triggered when the value is set to <strong>From</strong>.</td>
        </tr>
        <tr>
          <td><strong>Skip to End</strong></td>
          <td>Triggered when the value is set to <strong>To</strong>.</td>
        </tr>
        <tr>
          <td><strong>Value Increment</strong></td>
          <td>Triggered when the value starts increasing.</td>
        </tr>
        <tr>
          <td><strong>Value Decrement</strong></td>
          <td>Triggered when the value starts decreasing.</td>
        </tr>
      </tbody>
    </table>
  </div>
</details>

#### Parameters

- **From**: Starting value of the cycle.
- **To**: Ending value of the cycle.
- **Step**: Determines the increment or decrement applied to the value at each step.
- **Smooth Step**: Enables smooth transitions throughout the **Step Duration**.
- **Step Duration**: Time (in seconds) for each step.
- **Single Step**: Triggers one step manually, moving the value up or down once.

#### Playback Modes

- The **Cycle node** offers three playback modes:
  1. **Once**: The cycle runs from **From** to **To** in steps and stops.
  2. **Repeat**: The cycle loops, restarting at **From** after reaching **To**.
  3. **Ping-Pong**: The cycle alternates direction, moving from **From** to **To**, then back to **From**, and so on.

#### Controls

- **Start/Pause**: Toggles between starting and pausing the cycle.
- **Stop**: Ends the cycle and resets the value to **From**.
- **Skip to Start**: Immediately sets the value to **From**. If the cycle is running, it continues.
- **Skip to End**: Immediately sets the value to **To**. If the cycle is running, it continues.

### Operators

Operators in I3T perform calculations and manipulations on various data types, such as matrices, vectors, quaternions, and floats. They are categorized into different groups based on the type of operation they perform. Operators can be added through the **Workspace menu** and connected to other nodes to achieve specific results.

  <img src="../Docs/assets/operators.png?raw=true" height="250" alt="">

<details>
  <summary style="font-weight: bold; cursor: pointer;">Transformation  Operators</summary>

  <p><strong>Transformation Operators:</strong><br>These operators are used to modify the position, rotation, and scale of objects in 3D space.</p>
  <table>
    <thead>
      <tr>
        <th><strong>Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Translate</strong></td>
        <td>Moves objects along the X, Y, and Z axes.</td>
      </tr>
      <tr>
        <td><strong>EulerAngleX</strong></td>
        <td>Rotates objects around the X-axis.</td>
      </tr>
      <tr>
        <td><strong>EulerAngleY</strong></td>
        <td>Rotates objects around the Y-axis.</td>
      </tr>
      <tr>
        <td><strong>EulerAngleZ</strong></td>
        <td>Rotates objects around the Z-axis.</td>
      </tr>
      <tr>
        <td><strong>Rotate</strong></td>
        <td>Generic rotation in 3D space.</td>
      </tr>
      <tr>
        <td><strong>Scale</strong></td>
        <td>Scales objects along the X, Y, and Z axes.</td>
      </tr>
    </tbody>
  </table>

  <p><strong>Projection Operators:</strong><br>These operators generate matrices for viewing transformations.</p>
  <table>
    <thead>
      <tr>
        <th><strong>Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Orthogonal</strong></td>
        <td>Creates an orthogonal projection matrix.</td>
      </tr>
      <tr>
        <td><strong>Perspective</strong></td>
        <td>Generates a perspective projection matrix.</td>
      </tr>
      <tr>
        <td><strong>Frustum</strong></td>
        <td>Constructs a frustum projection matrix.</td>
      </tr>
      <tr>
        <td><strong>LookAt</strong></td>
        <td>Creates a view matrix based on eye, center, and up vectors.</td>
      </tr>
    </tbody>
  </table>
</details>
&nbsp;

<details>
  <summary style="font-weight: bold; cursor: pointer;">Matrix Operators</summary>

  <table>
    <thead>
      <tr>
        <th><strong>Matrix Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Matrix</strong></td>
        <td>Basic matrix initialization and operations.</td>
      </tr>
      <tr>
        <td><strong>Inversion</strong></td>
        <td>Computes the inverse of a matrix.</td>
      </tr>
      <tr>
        <td><strong>Transpose</strong></td>
        <td>Flips the rows and columns of a matrix.</td>
      </tr>
      <tr>
        <td><strong>Determinant</strong></td>
        <td>Calculates the determinant of a matrix.</td>
      </tr>
      <tr>
        <td><strong>Mat * Mat</strong></td>
        <td>Multiplies two matrices together.</td>
      </tr>
      <tr>
        <td><strong>Mat * Vec4</strong></td>
        <td>Multiplies a matrix by a 4D vector.</td>
      </tr>
      <tr>
        <td><strong>Vec4 * Mat</strong></td>
        <td>Multiplies a 4D vector by a matrix.</td>
      </tr>
      <tr>
        <td><strong>Mat + Mat</strong></td>
        <td>Adds two matrices together.</td>
      </tr>
    </tbody>
  </table>
</details>
&nbsp;

<details>
  <summary style="font-weight: bold; cursor: pointer;">Vec3 Operators</summary>
  <table>
    <thead>
      <tr>
        <th><strong>Vec3 Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Vec3</strong></td>
        <td>Creates or manipulates a 3D vector.</td>
      </tr>
      <tr>
        <td><strong>Show Vec3</strong></td>
        <td>Displays the components of a 3D vector.</td>
      </tr>
      <tr>
        <td><strong>Vec3 x Vec3</strong></td>
        <td>Computes the cross product of two 3D vectors.</td>
      </tr>
      <tr>
        <td><strong>Vec3 · Vec3</strong></td>
        <td>Calculates the dot product of two 3D vectors.</td>
      </tr>
      <tr>
        <td><strong>Vec3 + Vec3</strong></td>
        <td>Adds two 3D vectors together.</td>
      </tr>
      <tr>
        <td><strong>Vec3 - Vec3</strong></td>
        <td>Subtracts one 3D vector from another.</td>
      </tr>
      <tr>
        <td><strong>Float * Vec3</strong></td>
        <td>Multiplies a 3D vector by a scalar value.</td>
      </tr>
      <tr>
        <td><strong>Normalize Vec3</strong></td>
        <td>Converts a 3D vector to unit length.</td>
      </tr>
      <tr>
        <td><strong>Length(Vec3)</strong></td>
        <td>Computes the magnitude of a 3D vector.</td>
      </tr>
      <tr>
        <td><strong>Mix Vec3</strong></td>
        <td>Blends two 3D vectors together.</td>
      </tr>
    </tbody>
  </table>
</details>
&nbsp;

<details>
  <summary style="font-weight: bold; cursor: pointer;">Vec4 Operators</summary>
  <table>
    <thead>
      <tr>
        <th><strong>Vec4 Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Vec4</strong></td>
        <td>Creates or manipulates a 4D vector.</td>
      </tr>
      <tr>
        <td><strong>Vec4 · Vec4</strong></td>
        <td>Calculates the dot product of two 4D vectors.</td>
      </tr>
      <tr>
        <td><strong>Vec4 + Vec4</strong></td>
        <td>Adds two 4D vectors together.</td>
      </tr>
      <tr>
        <td><strong>Vec4 - Vec4</strong></td>
        <td>Subtracts one 4D vector from another.</td>
      </tr>
      <tr>
        <td><strong>Float * Vec4</strong></td>
        <td>Multiplies a 4D vector by a scalar value.</td>
      </tr>
      <tr>
        <td><strong>Normalize Vec4</strong></td>
        <td>Converts a 4D vector to unit length.</td>
      </tr>
      <tr>
        <td><strong>Perspective Division</strong></td>
        <td>Applies perspective division to a 4D vector.</td>
      </tr>
      <tr>
        <td><strong>Mix Vec4</strong></td>
        <td>Blends two 4D vectors together.</td>
      </tr>
    </tbody>
  </table>
</details>
&nbsp;

<details>
  <summary style="font-weight: bold; cursor: pointer;">Quaternion Operators</summary>
  <table>
    <thead>
      <tr>
        <th><strong>Quaternion Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Quat</strong></td>
        <td>Creates or manipulates a quaternion.</td>
      </tr>
      <tr>
        <td><strong>Quat(Float, Vec3)</strong></td>
        <td>Generates a quaternion using a float and a 3D vector.</td>
      </tr>
      <tr>
        <td><strong>Quat(Angle, Axis)</strong></td>
        <td>Constructs a quaternion from an angle and axis.</td>
      </tr>
      <tr>
        <td><strong>Quat(Vec3, Vec3)</strong></td>
        <td>Creates a quaternion between two 3D vectors.</td>
      </tr>
      <tr>
        <td><strong>Quat → Float, Vec3</strong></td>
        <td>Converts a quaternion to a float and a 3D vector.</td>
      </tr>
      <tr>
        <td><strong>Quat → Angle, Axis</strong></td>
        <td>Converts a quaternion to an angle and axis representation.</td>
      </tr>
      <tr>
        <td><strong>Float * Quat</strong></td>
        <td>Multiplies a quaternion by a scalar value.</td>
      </tr>
      <tr>
        <td><strong>Quat * Quat</strong></td>
        <td>Multiplies two quaternions together.</td>
      </tr>
      <tr>
        <td><strong>Quat → Euler</strong></td>
        <td>Converts a quaternion to Euler angles.</td>
      </tr>
      <tr>
        <td><strong>Euler → Quat</strong></td>
        <td>Converts Euler angles to a quaternion.</td>
      </tr>
      <tr>
        <td><strong>Slerp</strong></td>
        <td>Performs spherical linear interpolation between two quaternions.</td>
      </tr>
      <tr>
          <td><strong>Long Way Slerp</strong></td>
          <td>Performs spherical linear interpolation with an inverted path.</td>
      </tr> 
      <tr>
          <td><strong>Lerp</strong></td>
          <td>Performs linear interpolation between two quaternions.</td>
      </tr>
      <tr>
          <td><strong>Quat Conjugate</strong></td>
          <td>Computes the conjugate of a quaternion.</td>
      </tr>
      <tr>
          <td><strong>QVQ*</strong></td>
          <td>Performs quaternion-vector multiplication.</td>
      </tr>
      <tr>
          <td><strong>Inverse Quat</strong></td>
          <td>Computes the inverse of a quaternion.</td>
      </tr>
      <tr>
          <td><strong>Normalize Quat</strong></td>
          <td>Converts a quaternion to unit length.</td>
      </tr>
      <tr>
          <td><strong>Length(Quat)</strong></td>
          <td>Computes the magnitude of a quaternion.</td>
      </tr> 
    </tbody>
  </table>
</details>
&nbsp;

<details>
  <summary style="font-weight: bold; cursor: pointer;">Float Operators</summary>
  <table>
    <thead>
      <tr>
        <th><strong>Float Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Float</strong></td>
        <td>Creates or manipulates a floating-point value.</td>
      </tr>
      <tr>
        <td><strong>Clamp Float</strong></td>
        <td>Restricts a float to a specified range.</td>
      </tr>
      <tr>
        <td><strong>Float * Float</strong></td>
        <td>Multiplies two floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Float / Float</strong></td>
        <td>Divides one floating-point value by another.</td>
      </tr>
      <tr>
        <td><strong>Float + Float</strong></td>
        <td>Adds two floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Float ^ Float</strong></td>
        <td>Raises one float to the power of another.</td>
      </tr>
      <tr>
        <td><strong>Mix Float</strong></td>
        <td>Blends two floating-point values together.</td>
      </tr>
      <tr>
        <td><strong>Sin & Cos</strong></td>
        <td>Computes the sine and cosine of an angle.</td>
      </tr>
      <tr>
        <td><strong>Asin & Acos</strong></td>
        <td>Computes the arcsine and arccosine of a value.</td>
      </tr>
      <tr>
        <td><strong>Signum</strong></td>
        <td>Determines the sign of a floating-point value.</td>
      </tr>
    </tbody>
  </table>
</details>
&nbsp;

<details>
  <summary style="font-weight: bold; cursor: pointer;">Conversion Operators</summary>
  <table>
    <thead>
      <tr>
        <th><strong>Conversion Operator</strong></th>
        <th><strong>Description</strong></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><strong>Mat → TR</strong></td>
        <td>Converts a matrix to translation and rotation components.</td>
      </tr>
      <tr>
        <td><strong>TR → Mat</strong></td>
        <td>Constructs a matrix from translation and rotation components.</td>
      </tr>
      <tr>
        <td><strong>Mat → Vec4</strong></td>
        <td>Converts a matrix to a 4D vector.</td>
      </tr>
      <tr>
        <td><strong>Mat → Quat</strong></td>
        <td>Converts a matrix to a quaternion.</td>
      </tr>
      <tr>
        <td><strong>Mat → Floats</strong></td>
        <td>Converts a matrix to floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Vec4 → Mat</strong></td>
        <td>Converts a 4D vector to a matrix.</td>
      </tr>
      <tr>
        <td><strong>Vec4 → Vec3</strong></td>
        <td>Reduces a 4D vector to a 3D vector.</td>
      </tr>
      <tr>
        <td><strong>Vec4 → Floats</strong></td>
        <td>Converts a 4D vector to floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Vec3 → Mat</strong></td>
        <td>Converts a 3D vector to a matrix.</td>
      </tr>
      <tr>
        <td><strong>Vec3 → Vec4</strong></td>
        <td>Expands a 3D vector to a 4D vector.</td>
      </tr>
      <tr>
        <td><strong>Vec3 → Floats</strong></td>
        <td>Converts a 3D vector to floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Quat → Floats</strong></td>
        <td>Converts a quaternion to floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Floats → Mat</strong></td>
        <td>Creates a matrix from floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Floats → Vec4</strong></td>
        <td>Creates a 4D vector from floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Floats → Vec3</strong></td>
        <td>Creates a 3D vector from floating-point values.</td>
      </tr>
      <tr>
        <td><strong>Floats → Quat</strong></td>
        <td>Creates a quaternion from floating-point values.</td>
      </tr>
    </tbody>
  </table>
</details>

## Scene View

The **Scene View** provides a real-time 3D visualization of your scene, allowing you to see the effects of transformations, manipulations, and node connections directly as you build them in the **Workspace**.


### Manipulators

Manipulators are tools for adjusting transformations like translate, rotate, and scale. These manipulators can only modify existing transformations—they cannot create new transformations or connect them to models.
<div style="text-align: center;">
  <img src="../Docs/assets/manipulators.png?raw=true" height="350" alt="">
</div>

#### Visual Indicators in Scene View

- **Orange Outline**: Highlights the currently selected model in the **Scene View**.
- **Blue Outline**: Appears when a transformation selected in the **Workspace** affects a model in the **Scene View**. This blue outline serves as an indicator and not as a selection.

#### Types of Manipulators

<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px">Translate Manipulator</summary>
  <ul style="margin-left: 40px;">
    <img src="../Docs/assets/translate.png?raw=true" width="200" alt="">
    <li>Shown as colored arrows (Red: X-axis, Green: Y-axis, Blue: Z-axis).</li>
    <li>Drag the arrows to move the object along a specific axis.</li>
  </ul>
</details>

<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px">Rotate Manipulator</summary>
  <ul style="margin-left: 40px;">
    <img src="../Docs/assets/rotate.png?raw=true" width="200" alt="">
    <li>Appears as colored circles around the object (Red: X-axis, Green: Y-axis, Blue: Z-axis).</li>
    <li>Click and drag a circle to rotate the object around the corresponding axis.</li>
  </ul>
</details>

<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px">Scale Manipulator</summary>
  <ul style="margin-left: 40px;">
    <img src="../Docs/assets/scale.png?raw=true" width="190" alt="">
    <li>Displayed as colored dots (Red: X-axis, Green: Y-axis, Blue: Z-axis).</li>
    <li>Drag the squares to scale the object along a specific axis.</li>
  </ul>
</details>

#### Using Manipulators

<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px">Enable Manipulators</summary>
  <ul style="margin-left: 40px;">
    <li>Manipulators are displayed for selected transformations in the <strong>Workspace</strong>, not automatically for selected models in the <strong>Scene View</strong>.</li>
    <li>To enable manipulators, select a transformation in the <strong>Workspace</strong>.</li>
    <li>Manipulators can also be toggled off from the menu bar:
      <ul>
        <li><strong>Settings > Manipulators</strong>: Disable manipulators through the Scene View menu bar.</li>
        <li><strong>Toolbar</strong>: Use the buttons located below the main menu bar to toggle manipulators.</li>
      </ul>
    </li>
  </ul>
</details>

<details>
  <summary style="font-weight: bold; cursor: pointer; margin-left: 20px">Adjusting Values</summary>
  <ul style="margin-left: 40px;">
    <li>Changes made via manipulators are reflected directly in the selected transformation matrices in the <strong>Workspace</strong> window and also next to the model in <strong>Scene View</strong>.</li>
  </ul>
</details>


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

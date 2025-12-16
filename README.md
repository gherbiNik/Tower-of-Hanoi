# 3D Tower of Hanoi - Computer Graphics Project

This project is a 3D interactive implementation of the classic "Tower of Hanoi" mathematical puzzle, developed in C++ using **OpenGL** and **GLUT**. The graphics engine was built from scratch to handle 3D scenes, lighting, textures, and complex game logic.

## Key Features

The project meets and exceeds the Computer Graphics course requirements through the following features:

### Gameplay & Logic
* **Complete Game Logic**: Rigorous implementation of Tower of Hanoi rules (e.g., larger disks cannot be placed on smaller ones).
* **Interactive Selection**: Intuitive keyboard control system to select pegs, pick up, and drop disks.
* **Undo/Redo System**: Stack-based (LIFO) system allowing moves to be undone (`Undo`) and redone (`Redo`), saving the full game state.
* **Reset**: Functionality to reload the entire scene and initial state.

### Rendering & Graphics
* **Structured 3D Scene**: Asset and scene loading via `.ovo` format, with textures mapped onto the table and environment.
* **Advanced Lighting**:
    * **Static Lights**: Omnidirectional lights for ambient illumination.
    * **Dynamic Light**: An **interactive Spotlight** that follows the player's selection, illuminating the active peg and providing immediate visual feedback.
* **Planar Reflections**: Real-time specular reflections on the table surface (implemented using `glFrontFace` inversion and reflection matrices).
* **Optimization**: Stable framerate consistently above 24 FPS (displayed on screen).

### Camera Management
* **Free Camera**: Free movement in 3D space ("Fly" mode) with WASD controls for translation and rotation.
* **Camera Presets**: 4 predefined viewpoints (Front, Side, Top-Down, Isometric) for optimal puzzle viewing.

---

## Controls

### Game Controls
| Key | Action |
| :--- | :--- |
| **Left / Right Arrow** | Change selected peg (cyclic) |
| **Up Arrow** | Pick up disk from current peg |
| **Down Arrow** | Drop disk on current peg |
| **U** | **Undo**: Revert the last move |
| **Y** | **Redo**: Restore the last undone move |
| **R** | **Reset**: Restart game and reload scene |
| **ESC** | Exit application |

### Camera Controls
| Key | Action |
| :--- | :--- |
| **1, 2, 3, 4** | Switch to Camera Presets |
| **P** | Switch to Main Camera (Free Move) |
| **M** | Toggle **Movement** / **Rotation** mode |
| **W, A, S, D** | Move or Rotate camera (based on Mode M) |
| **Q, E** | Vertical movement / Roll |

---

## Technical Architecture

The project is built on a modular architecture:

1.  **Engine (Core)**:
    * Uses the **Singleton** pattern for global management and the **PIMPL** (Pointer to Implementation) pattern to hide low-level GLUT/OpenGL details.
    * Manages the rendering loop, input callbacks, and FPS calculation.

2.  **Scene Graph**:
    * Hierarchical node management (Mesh, Lights, Cameras) via the `Node` class.
    * Dynamic scene parsing: the code automatically identifies pegs and disks by analyzing node names in the 3D file (`Palo1`, `Disco1`, etc.).

3.  **Game Logic (`Hanoi` class)**:
    * Logically separated from rendering.
    * Manages disk state, move validation, and history for undo/redo.
    * Updates 3D model transformation matrices to reflect the logical state.

---

## System Requirements
* **Libraries**: OpenGL, FreeGLUT, FreeImage, GLM.
* **Compiler**: C++ compiler compatible with C++17.
* **Hardware**: Graphics card with standard OpenGL support.

## In-game screenshot
<img width="750" height="550" alt="image" src="https://github.com/user-attachments/assets/51381c63-7353-4fab-b606-38b01a2ba1bd" />



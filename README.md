# Firefighting Plane Simulation

This project is a 3D firefighting airplane simulation built using C++ and OpenGL. It allows users to control an airplane, navigate through a 3D environment, and interact with the scene. The project uses modern OpenGL techniques and several libraries to handle rendering, input, and mathematical operations.

## Features
- Realistic airplane movement and controls.
- Dynamic camera system for immersive views.
- Shader-based rendering for enhanced visual effects.
- Simulated environments with lighting and materials.
- Modular and reusable code structure.

## Libraries Used
The project relies on the following libraries:
1. **GLAD**: OpenGL function loader, used to manage OpenGL extensions.
2. **GLFW**: Library for creating windows, handling input, and managing OpenGL contexts.
3. **GLM**: Header-only library for mathematical operations, including vector and matrix transformations.
4. **Custom Shader Class**: Manages OpenGL shaders for rendering.
5. **Custom Camera Class**: Implements a 3D camera system for navigation.
6. **Custom Scene Management**: Handles the 3D environment and objects.

## Requirements
- C++14 or later.
- OpenGL 4.3 or higher.
- A modern C++ compiler (e.g., MSVC, GCC, or Clang).

## Installation
1. Clone the repository
2. Open the project in a IDE.
3. Ensure the required dependencies are included:
   - GLAD and GLFW are included in the project setup.
   - GLM is included in the `Libraries/include` directory.
4. Build the project using the provided solution file.

## File Structure
- `main.cpp`: Entry point of the application, handles the main game loop and input processing.
- `airplane.cpp` / `airplane.h`: Contains logic for airplane movement and behavior.
- `camera.cpp` / `camera.h`: Implements the 3D camera system.
- `scene.cpp` / `scene.h`: Manages the 3D environment and objects.
- `shader.cpp` / `shader.h`: Handles OpenGL shaders for rendering.
- `glm_includes.h`: Centralized inclusion of GLM headers.
- `basic.vs` / `basic.fs`: Vertex and fragment shaders for rendering.

## How to Run
1. Build the project.
2. Run the compiled executable.
3. Use the following controls to interact with the simulation:
   - `W`: Roll counter-clockwise.
   - `S`: Roll clockwise.
   - `A`: Pitch down (forward).
   - `D`: Pitch up (backward).
   - `Q`: Yaw left.
   - `E`: Yaw right.
   - `Space`: Accelerate.
   - `Left Shift`: Decelerate.
   - `Down Arrow`: Drop a blue cube.
   - `ESC`: Exit the simulation.

## Contributing
Contributions are welcome! Please fork the repository, make your changes, and submit a pull request.

## Acknowledgments
- **GLAD**: [https://github.com/Dav1dde/glad](https://github.com/Dav1dde/glad)
- **GLFW**: [https://www.glfw.org/](https://www.glfw.org/)
- **GLM**: [https://github.com/g-truc/glm](https://github.com/g-truc/glm)

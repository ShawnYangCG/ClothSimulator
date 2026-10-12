# ClothSimulator

An interactive cloth simulator built with **C++, Qt 5, and OpenGL**. A mass-spring system models cloth deformation under gravity, wind, and contact with the ground or a sphere.

## Features

- Mass-spring cloth simulation with interactive playback and frame review.
- Gravity and adjustable wind forces.
- Ground and sphere collision handling.
- Rotating sphere interaction with adjustable rotation speed.
- Two-corner pinning for suspended cloth.
- OBJ model loading and geometry saving.
- Frame capture to BMP images.

## Demos

### 1. Pinned Cloth, Ground Collision, and Wind

Two corners of the cloth are fixed while the remaining cloth falls under gravity and collides with the ground. Ground collision is then disabled, and the wind strength is adjusted to control the external force.

**Controls:** Enable `lock`, `gravity`, and `ground`, then press `play`. Disable `ground`, enable `wind`, and adjust `value`.

![Pinned cloth with ground collision and adjustable wind](asset/free_drop_wind.gif)

### 2. Free Fall onto a Sphere

The cloth falls freely under gravity, collides with a spherical obstacle, and drapes over its surface.

**Controls:** Enable `gravity`, `ground`, and `ball`, leave `lock` disabled, and press `play`.

![Cloth falling onto a sphere](asset/free_collision.gif)

### 3. Free Fall onto a Rotating Sphere

The cloth falls onto a rotating spherical obstacle, demonstrating cloth motion during contact with the moving surface.

**Controls:** Enable `gravity`, `ground`, `ball`, and `rotation`. Set `speed` to a nonzero value, leave `lock` disabled, and press `play`.

![Cloth falling onto a rotating sphere](asset/free_collision_rotation.gif)

## Environment Setup

| Component | Configuration |
| --- | --- |
| Platform | Windows |
| Build target | 32-bit x86, Release |
| Compiler | Visual Studio 2019, MSVC 14.29 / v142 |
| Windows SDK | 10.0.19041.0 |
| Qt | Qt 5.13.0, MSVC 2017 32-bit kit |
| Graphics | OpenGL |

Install the Visual Studio C++ desktop development tools and Windows SDK, then install the Qt kit. The Qt installation should include `qmake`, `moc`, `uic`, `rcc`, and `windeployqt`. Use the 32-bit MSVC Qt kit to match the build target.

## Project Structure

```text
ClothSimulator/
|-- asset/                       Demo GIFs
|-- renderWindow/                OpenGL rendering, camera interaction, OBJ I/O,
|                                and mathematical utilities
|-- testObj/                     Cloth meshes
|   `-- environment/             Sphere and ground models
|-- MainWindow.cpp / .h          File loading, interface, and simulation controls
|-- SimulationEnvironment.*      Spring connections, forces, and simulation setup
|-- SimulationObject.*           Vertex dynamics, integration, and collisions
|-- FieldOfForce.*               Gravity and wind forces
|-- FrameRecord.*                Simulation frame storage
|-- main.cpp                     Application entry point
|-- ClothSimulator_version_1.ui  Qt interface layout
|-- ClothSimulator.pro          qmake project configuration
|-- build-release.cmd            Windows build and deployment script
|-- build/                       Generated compilation output
`-- dist/                        Generated runnable application package
    |-- testObj/                 Runtime model assets
    `-- output/                  Captured BMP frames
```

## Build and Run

### Build

From the project directory, run the following command with the path to your Qt kit:

```bat
build-release.cmd "C:\Qt\5.13.0\msvc2017"
```

Alternatively, set `QTDIR` and run the script:

```bat
set "QTDIR=C:\Qt\5.13.0\msvc2017"
build-release.cmd
```

The script locates the Visual Studio C++ tools, builds the application, and copies the required Qt libraries, plugins, Microsoft C++ runtime libraries, and model assets into `dist`.

### Launch

Double-click the executable or run:

```bat
dist\ClothSimulator_version_1.exe
```

Keep the entire `dist` folder together when moving the application.

### Load and Simulate

1. Select **File → Open**.
2. Load `testObj/plane10x10.obj`, `plane15x15.obj`, or `plane20x20.obj` from the application folder.
3. Configure the simulation using the controls on the right.
4. Press **play** to start and **pause** to stop playback.
5. While paused, use the timeline slider to inspect recorded frames. Reload a cloth model to start a new simulation.

The simulation controls become available after loading a cloth model. The sphere and ground are loaded automatically from `testObj/environment`.

| Control | Function |
| --- | --- |
| `gravity` | Toggle gravity |
| `ground` | Toggle ground visibility and collision |
| `ball` | Toggle sphere visibility and collision |
| `lock` | Pin two corners along one edge of the cloth |
| `wind` | Enable wind and activate the `value` slider |
| `value` | Set wind strength from 0 to 20; the default is 0 |
| `rotation` | Enable sphere rotation after enabling `ball` |
| `speed` | Set rotation speed from 0 to 720 |
| `record` | Save playback frames to `output/render_*.bmp` |
| `File → Save` | Save the current cloth geometry as an OBJ file |

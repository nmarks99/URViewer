# URViewer
A no frills 3D viewer for Universal Robots based on [raylib](https://github.com/raysan5/raylib).

![URViewerScreenshot](https://github.com/user-attachments/assets/fb6a93a0-3423-4521-aa7e-01342208b9ba)

# Installation

**Dependencies:**
- [raylib](https://github.com/raysan5/raylib)
- [ur_rtde](https://gitlab.com/sdurobotics/ur_rtde)
- [EPICS Base](https://docs.epics-controls.org/en/latest/getting-started/installation.html)
- [Boost](https://www.boost.org/) (required by ur_rtde)

For now, no compiled binaries are provided for URViewer so you must build it from source.
It has only been tested on Linux, though compiling for other operating systems should be possible.
After installing the required dependencies, use cmake to build the URViewer application. Depending on
where you installed the dependencies, you may need to set `-Dur_rtde_DIR` and `-Draylib_DIR` when running cmake.

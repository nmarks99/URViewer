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

EPICS base and boost must be installed on your system. raylib and ur_rtde can be
cloned and built with the project using FetchContent by setting `-DFETCH_UR_RTDE=ON` and `-DFETCH_RAYLIB=ON` (default).
If you have installed these dependencies yourself, set those flags to OFF and you may need to set `-Dur_rtde_DIR` and `-Draylib_DIR`.

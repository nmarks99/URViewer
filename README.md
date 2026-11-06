# URViewer
A no frills 3D viewer for Universal Robots based on [raylib](https://github.com/raysan5/raylib).

![URViewerScreenshot](https://github.com/user-attachments/assets/fb6a93a0-3423-4521-aa7e-01342208b9ba)

# Installation

**Dependencies:**
- [raylib](https://github.com/raysan5/raylib)
- [ur_rtde](https://gitlab.com/sdurobotics/ur_rtde)
- [EPICS Base](https://docs.epics-controls.org/en/latest/getting-started/installation.html)
- [PVXS](https://github.com/epics-base/pvxs)
- [Boost](https://www.boost.org/) (required by ur_rtde)

EPICS Base, PVXS, and Boost must be installed on your system. Set `EPICS_BASE`,
`EPICS_HOST_ARCH`, and `PVXS` in the environment or pass `-DEPICS_BASE=...`,
`-DEPICS_HOST_ARCH=...`, and `-DPVXS=...` to CMake.

PACE is included with FetchContent. Raylib and ur_rtde are also cloned and built
with FetchContent by default. To use installed raylib or ur_rtde packages instead,
disable `FETCH_RAYLIB` or `FETCH_UR_RTDE` and provide its CMake package location.

## Building from Source

For now, no compiled binaries are provided for URViewer so you must build it from source.
It has only been tested on Linux, though compiling for other operating systems should be possible.

```bash
# make a build directory
mkdir build && cd build

# Configure with cmake. If you don't have sudo, set the install prefix
# ~/.local is a good choice
cmake .. \
    -DCMAKE_INSTALL_PREFIX=~/.local \
    -DPVXS=~/.local/epics/pvxs

# Compile...
make
# ...and optionally install
# make install
```

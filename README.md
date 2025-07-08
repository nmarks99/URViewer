# URViewer
A no frills 3D viewer for Universal Robots based on [raylib](https://github.com/raysan5/raylib).

![screenshot](https://github.com/user-attachments/assets/76936aec-1a7f-4306-9d05-366aed537e25)


# Installation

**Dependencies:**
- [raylib](https://github.com/raysan5/raylib)
- [ur_rtde](https://gitlab.com/sdurobotics/ur_rtde)
- [Boost](https://www.boost.org/) (required by ur_rtde)

For now, no compiled binaries are provided for URViewer so you must build it from source.
It has only been tested on Linux, though compiling for other operating systems should be possible.
After installing the required dependencies, use cmake to build the URViewer application. Depending on
where you installed the dependencies, you may need to set `-Dur_rtde_DIR` and `-Draylib_DIR` when running cmake.

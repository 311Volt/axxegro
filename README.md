# axxegro

A C++17 wrapper for [Allegro5](https://github.com/liballeg/allegro5) that aims to combine
Allegro's straightforward and intuitive API with modern C++. It is not a complete wrapper and exposes the Allegro API to the user rather
than redefining every constant; however, the goal for 1.0 is to completely 
eliminate the need to call any Allegro functions directly.

The library is work-in-progress and it is still missing some things, but it's
already usable for some simple applications, getting the same job done with far less
code and close to zero overhead.

Work-in-progress also means that the API is completely unstable and might change
on any scale at any time.

# examples

```c++
al::Bitmap lena("lena.jpg");
lena.draw({0, 0});
```

```c++
al::EventLoop loop = al::EventLoop::Basic(); //with pre-registered event sources
loop.loopBody = [](){
    al::TargetBitmap.clearToColor(al::RGB(0,0,0));
    // draw stuff
    al::CurrentDisplay.flip();
};
loop.enableClock(60); //framerate limiter (60 fps)
loop.run(); //will run until window is closed
```

For more examples, see examples/src.

# how to build with CMake
Use:   
```bash
mkdir build
cd build
cmake ..
make # -jXX
```

By default, example programs will be built. Disable `AXXEGRO_BUILD_EXAMPLES`
or build just the `axxegro` target if you don't want that.

Any programs that use axxegro will also need Allegro linked separately, including
the examples. 
If you're using Linux, you can likely install Allegro5 with your distribution's package manager.
Alternatively, you can supply your own build of Allegro. To do this, place as necessary:
- library files in `deps/lib`
- headers in `deps/include`
- DLLs in `examples/dll` (for Windows)


For Windows builds, the most recent MinGW-w64 based build of [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) is recommended. (tip for VSCode users: make a copy of the toolset's `mingw32-make.exe` and call it `make.exe` so that `cmake.generator` can be set automatically)

# using the library without CMake
You can also ignore the CMake stuff and simply add axxegro's sources and `include`
as an include directory to your Allegro project. axxegro is ISO C++ and does not,
nor will it ever need any special build options or code generation scripts.


# development progress

As I'm writing this, the library is about 60% done.
Everything in Allegro has an axxegro equivalent, except
the following missing things:

### high priority
 - everything audio
 - a subset of gfx routines (pixel formats, blending modes)
 - vertex/index buffers
 - platform-specific and D3D/OpenGL specific functions
 - native dialog menus
 - video streaming

### medium priority
 - virtual filesystem, PhysicsFS and memfile
 - haptic
 - joystick
 - state
 - system routines
 - touch input

### low priority / won't be done
 - fixed point math (consider this instead: https://github.com/MikeLankamp/fpm)
 - memory (al_malloc et al.) - shouldn't be needed at all
 - threads (C++ has std::thread)
 - path structures (C++ has std::filesystem::path)


# docs
Documentation will be generated and placed in `build/docs` if Doxygen is found
and `AXXEGRO_BUILD_DOCS` is set to ON (which is the default)

# credits

axxegro builds on the excellent work of the creators of:

 - Allegro 5: https://github.com/liballeg/allegro5
 - libfmt: https://github.com/fmtlib/fmt
 - tcbrindle/span: https://github.com/tcbrindle/span
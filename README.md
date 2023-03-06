# axxegro

A C++20 header-only wrapper for the game programming library [Allegro5](https://github.com/liballeg/allegro5) 
that aims to combine Allegro's straightforward and intuitive API with modern C++. It is not a complete wrapper 
and exposes the Allegro API to the user rather than redefining every constant; however, 
the goal for 1.0 is to completely eliminate the need to call any Allegro functions directly.
To use the library currently, the user is assumed to have good knowledge of Allegro's C API.
A series of tutorials will be written in the future to make axxegro accessible.

The library is work-in-progress and it is still missing some things, but it's
already usable for simple applications, getting the same job done with far less
code and close to zero overhead.

Work-in-progress also means that the API is completely unstable and might change
on any scale at any time.

# main features so far

- Wrappers for most of Allegro's API
- Built-in homebrew `Vec*` and `Rect` classes for screen coordinate manipulation
- An `EventLoop` class that handles boilerplate for basic programs
- Template-powered user event support
- `constexpr` colors and helper functions
- exceptions for common failure points (no more mysterious segfaults 
    when you forget to `al_init()` or misspell a filename)


# examples

For complete example programs, see [examples/src](examples/src).

```c++
al::Display display(800, 600);

al::Bitmap lena("lena.jpg");
lena.draw({0, 0});
```

```c++
al::EventLoop loop = al::EventLoop::Basic(); //with pre-registered event sources
loop.enableEscToQuit();
loop.loopBody = [](){
    al::TargetBitmap.clearToColor(al::Black);
    handleInput();
    render();
    al::CurrentDisplay.flip();
};
loop.enableFramerateLimit(60); //limit to 60 fps
loop.run(); //will run until window is closed
```
# Compiler support

 - GCC: >= 13
 - Clang: >= 15 (`-fexperimental-library` required)
 - MSVC: >= 19.29 (VS 2019 16.10) (**not tested**)

Support for GCC 10 will be brought back soon.

# Building

Examples and documentation can be built with CMake:

```bash
mkdir build
cd build
cmake ..
make #-jX
```

If Allegro can be found by CMake, no further configuration is required.
If you prefer to not install Allegro to your toolchain/system path, you may
specify a custom `CMAKE_PREFIX_PATH`.

# Integration

Since axxegro is plain C++20, it is enough to copy the headers to a working Allegro5 project
and compile with `-I<path_to_axxegro>`.

It is also supported to use axxegro as a subdirectory in a CMake project:
```cmake
add_subdirectory(axxegro)
target_link_libraries(my_game axxegro)
```
Installation and package config files are still TODO.


# development progress

As I'm writing this, the library is about 80% done.
Everything in Allegro has an axxegro equivalent, except
the following missing things:

### high priority
 - a subset of gfx routines (blending modes, sub-bitmaps)
 - platform-specific and D3D/OpenGL specific functions
 - native dialog menus
 - video streaming

### medium priority
 - virtual filesystem, PhysicsFS and memfile
 - some audio features (devices etc)
 - haptic
 - joystick
 - state
 - system routines
 - touch input
 - make addons optional like in plain allegro

### low priority / won't be done
 - fixed point math (consider this instead: https://github.com/MikeLankamp/fpm)
 - threads (C++ has std::thread)
 - path structures (C++ has std::filesystem::path)


# docs

(WARNING docs are very incomplete at the moment and will probably move to
something other than doxygen)

Documentation will be generated and placed in `build/docs` if Doxygen is found
and `AXXEGRO_BUILD_DOCS` is set to ON (which is the default for `AXXEGRO_MASTER_PROJECT`)

# credits

axxegro builds on the excellent work of the creators of Allegro 5: https://github.com/liballeg/allegro5

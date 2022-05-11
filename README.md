# axxegro

A C++17 wrapper for [Allegro5](https://github.com/liballeg/allegro5) that aims to combine
Allegro's straightforward and intuitive API with modern C++. It is not a complete wrapper and exposes the Allegro API to the user rather
than redefining every constant; however, the goal for 1.0 is to completely 
eliminate the need to call any Allegro functions directly.

The library is work-in-progress and it is still missing a lot of things, but it's
already usable for some simple applications, getting the same job done with far less
code and close to zero overhead.

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


# how to build with CMake
Make sure to clone this project with submodules. Use:   
```bash
mkdir build
cd build
cmake ..
make # -jXX
```

Any programs that use axxegro will also need Allegro linked separately, including
the examples. 

If you're using Linux, you can likely install [Allegro5](https://github.com/liballeg/allegro5) with your distribution's package manager.

Alternatively, you can supply your own build of Allegro. To do this, place as necessary:
- library files in `deps/lib`
- headers in `deps/include`
- DLLs in `examples/dll` (for Windows)


For Windows builds, the most recent MinGW-w64 based build of [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) is recommended. (tip for VSCode users: make a copy of the toolset's `mingw32-make.exe` and call it `make.exe` so that `cmake.generator` can be set automatically)

# using the library without CMake
You can also ignore the CMake stuff and simply add axxegro's sources and `include`
as an include directory to your Allegro project. axxegro is ISO C++ and does not,
nor will it ever need any special build options or code generation scripts.

# docs
Documentation will be generated and placed in `build/docs` if Doxygen is found
and `AXXEGRO_BUILD_DOCS` is set to ON (which is the default)
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


# how to build
1. build or install [Allegro5](https://github.com/liballeg/allegro5) and [{fmt}](https://github.com/fmtlib/fmt) first 

     *(note: a build of allegro5 is needed for the example programs, but not for the library itself - the user links against a5 separately)*

2. where necessary, place:
    - library files in `deps/lib`
    - headers in `deps/include`
    - (Windows) DLLs in `examples/dll`

3. use `cmake -Bbuild -H. && cd build && make`

For Windows builds, the most recent MinGW-w64 based build of [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) is recommended. (tip for VSCode users: make a copy of the toolset's `mingw32-make.exe` and call it `make.exe` so that `cmake.generator` can be set automatically)

# linking
Link against this library as well as Allegro5 itself.




# docs
Documentation in doxygen compatible format is contained within header files.
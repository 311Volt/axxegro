# axxegro

A C++17 wrapper for [Allegro5](https://github.com/liballeg/allegro5) that
aims to provide a complete replacement for its C API.

Barely anything in here yet, this is work in progress

# how to build
1. build [Allegro5](https://github.com/liballeg/allegro5) and [{fmt}](https://github.com/fmtlib/fmt) first

2. place the built libraries in deps/lib and headers in deps/include

3. use `cmake . && cd build && make`

For Windows builds, the most recent MinGW-w64 based build of [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) is recommended.

# how to use
Link against this library as well as Allegro5 itself.

# docs
Header files are documented. Doxygen can be used to create an HTML version.
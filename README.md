# axxegro

A C++17 wrapper for [Allegro5](https://github.com/liballeg/allegro5) that aims to combine
Allegro's straightforward and intuitive UI with modern C++ safety and lifetime management
techniques. It is not a complete wrapper and exposes the Allegro API to the user rather
than redefining every `struct` and `define`; however, the goal for 1.0 is to completely 
eliminate the need to call any Allegro functions directly.

Barely anything in here yet, this is work in progress. This library is developed in
parallel to [lpg-gui](https://github.com/311Volt/lpg-gui) (repo coming soon), which means
that features needed for lpg-gui are prioritized.

# how to build
1. build [Allegro5](https://github.com/liballeg/allegro5) and [{fmt}](https://github.com/fmtlib/fmt) first 

     *(note: a build of allegro5 is needed for the example programs, but not for the library itself - the user links against a5 separately)*

2. place the built libraries in deps/lib and headers in deps/include

3. use `cmake -Bbuild -H. && cd build && make`

For Windows builds, the most recent MinGW-w64 based build of [TDM-GCC](https://jmeubank.github.io/tdm-gcc/download/) is recommended.

# how to use
Link against this library as well as Allegro5 itself.

# docs
Documentation in doxygen format is contained within header files.
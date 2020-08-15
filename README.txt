libImGui
========

A very small C++ library based on Dear ImGui, intended to make it easier to create applications by copying one of the samples (only one at the time of writing).

This library is intended to be consumed as a Git submodule, though it may be properly packaged later one.

Clone https://github.com/JPGygax68/libImGui.git, and make sure that its submodules are initialized/updated (libImGui in turn consumes Dear ImGui as a submodule).

To create an application, go to the samples subdirectory and copy both CMakeLists.txt and one of the source files, e.g. hello.cpp, then adapt CMakeLists.txt.


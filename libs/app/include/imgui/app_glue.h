#pragma once

#include <utility>

struct Extents
{
    int w, h;
};
struct Bounds
{
    int x, y;
    int w, h;
};

struct WindowHandle
{
    void *window;
    void *gl_context;
};

// TODO: replace this collection of function declarations with an abstract base class,
// and use the "final" keyword on implementations to enable to the devirtualization
// optimization.

// Platform

void imgapp_initPlatform();
void imgapp_deinitPlatform();

// TODO: some graphic libraries may need global initialization, some per-window.
//   => extend the interface to support both

void imgapp_initGraphicLibrary(const char *version);
void imgapp_deinitGraphicLibrary();

auto imgapp_getMainMonitorDPI() -> float;
auto imgapp_getMainDisplayExtents() -> Extents;
// auto imgapp_getMainDisplayBounds() -> Bounds;

/* Must return true unless the application has been requested to terminate.
 */
bool imgapp_pumpEvents();

void imgapp_newFrame(void *window);
void imgapp_clearFrame(const ImVec4 &color);
void imgapp_renderGui();
void imgapp_presentFrame(void *window);

// void imgapp_initWindowForGraphicLibrary(void *window, void *gl_context);

auto imgapp_openWindow(const char *title, int win_w, int win_h) -> WindowHandle;
void imgapp_destroyWindow(void *window, void *gl_context);

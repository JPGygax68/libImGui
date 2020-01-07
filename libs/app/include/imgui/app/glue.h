#pragma once

#include <utility>

const int DEFAULT_POS = -32768;

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

void imgapp_init();
void imgapp_shutdown();

float imgapp_getMainMonitorDPI();
auto imgapp_getMainDisplayExtents() -> Extents;
// auto imgapp_getMainDisplayBounds() -> Bounds;

auto imgapp_openWindow(const char *title, int win_w, int win_h) -> WindowHandle;
void imgapp_destroyWindow(void *window, void *gl_context);

void imgapp_initWindowForOpenGL(void *window, void *gl_context);

/* Must return true unless the application has been requested to terminate.
 */
bool imgapp_pumpEvents();

void imgapp_newFrame(void *window);

void imgapp_presentFrame(void *window);
#pragma once

#include <utility>


const int DEFAULT_POS = -32768;

struct Extents { int w, h; };
struct Bounds { int x, y; int w, h; };
struct WindowHandle { void* window; void* gl_context; };


void app_init();
void app_shutdown();

float app_getMainMonitorDPI();
auto app_getMainDisplayExtents() -> Extents;
// auto app_getMainDisplayBounds() -> Bounds;

auto app_openWindow(const char *title, int win_w, int win_h) -> WindowHandle;
void app_destroyWindow(void *window, void *gl_context);

void app_initWindowForOpenGL(void *window, void *gl_context);

/* Must return true unless the application has been requested to terminate.
 */
bool app_pumpEvents(void *window);

void app_newFrame(void *window);

void app_presentFrame(void *window);
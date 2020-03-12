#pragma once

#include <cstdint>

class Platform_adapter
{
public:
    static const int UNDEFINED_POS = -32768;
    static const int UNDEFINED_SIZE = 32767;

    struct Extents
    {
        int w, h;
    };
    struct ExtentsF
    {
        float w, h;
    };
    struct Bounds
    {
        int x, y;
        int w, h;
    };

    /* Performs global initialization of backend libraries and/or resources.
     */
    virtual void init() = 0;

    /* Properly shuts down all backend libraries, returning all their resources.
     */
    virtual void shutdown() = 0;

    /* Creates a "context" representing the specified window vis-a-vis the graphics library,
     * and returns it as an opaque pointer.
     */
    virtual auto createGraphicsContext(void *window) -> void * = 0;

    /* Destroys a graphics context created by createGraphicsContext().
     * For maximum flexibility, this method is given both the window and the graphics
     * context handles.
     */
    virtual void destroyGraphicsContext(void *window, void *context) = 0;

    /* Returns the number of monitors known to the computer. Note that "known" does not imply
     * "available"; use isMonitorReady() to find out if a given monitor is available.
     */
    virtual int getNumberOfMonitors() = 0;

    /* Determine whether or not the monitor with the specified index is available.
     */
    virtual bool isMonitoryReady(int index) = 0;

    /* Return the bounds of the specified monitor: its position (x, y) in the virtual
     * multi-screen space, and its size (w, h).
     */
    virtual auto getMonitorBounds(int index) -> Bounds = 0;

    /* Returns the pixels density, in dots per inch, of the specified monitor.
     */
    virtual auto getMonitorDPI(int index) -> float = 0;

    /* Returns the physical size of a monitor, in inches.
     */
    virtual auto getMonitorPhysicalSize(int index) -> ExtentsF;

    /* Open a window.
     * title    Window caption. Can be set to nullptr for fullscreen.
     * w, h     Size of the window in pixels. Leaving both sizes = 0 will open
     *          a full-screen window.
     * x, y     Position of the window, in pixels in virtual screen space.
     *          Can be set to UNDEFINED_POS to let the system choose.
     *          Relative to position of specified monitor.
     * flags    TBD
     * monitor  Monitor index. The window may however actually appear on another
     *          monitor if x, y are != 0 (virtual screen space).
     */
    virtual auto openWindow(const char *title, int w, int h,
                            int x = UNDEFINED_POS, int y = UNDEFINED_POS,
                            uint32_t flags = 0, int monitor = 0)
        -> void * = 0;

    /* Destroy the specified window and its associated graphics context.
     */
    virtual void destroyWindow(void *window, void *gl_context) = 0;

    /* Fetch and dispatch pending input events. Returns true as long as the application
     * wants to keep running, false when it must terminate.
     */
    virtual bool pumpEvents() = 0;

    /* Set the "clear" color (background color that will be set when beginNewFrame()
     * is called with clear = true).
     */
    virtual void setClearColor(float r, float g, float b, float a = 0) = 0;

    /* Begin a new frame on the specified window and with the specified graphics context.
     * May return an opaque handle to a "frame context" that must be passed to
     * renderGui() and presentFrame().
     * TODO: clarify if/how multi-threading can be supported. May require an extra method
     * to create a copy of a graphics context.
     */
    virtual auto beginNewFrame(void *window, void *gl_context, bool clear = true) -> void * = 0;

    virtual void renderGui(void *frame_context) = 0;

    virtual void presentFrame(void *frame_context) = 0;
};
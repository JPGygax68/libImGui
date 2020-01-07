#include <cstdio>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"

#include "glue.h"

void imgapp_init()
{
    // Setup window
    glfwSetErrorCallback([](int error, const char *description) { fprintf(stderr, "Error %d: %s\n", error, description); });
    if (!glfwInit())
        exit(1);

        // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
}

void imgapp_shutdown()
{
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();
}

float imgapp_getMainMonitorDPI()
{
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    int monitor_width, monitor_height;
    glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &monitor_width, &monitor_height);

    float hdpi = (float)mode->width / ((float)monitor_width / 25.4f);
    float vdpi = (float)mode->height / ((float)monitor_height / 25.4f);

    return hdpi; // we choose the horizontal DPIs here
}

auto imgapp_getMainDisplayExtents() -> Extents
{
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    return {mode->width, mode->height};
}

auto imgapp_openWindow(const char *title, int win_w, int win_h) -> WindowHandle
{
    WindowHandle handle;

    handle.window = glfwCreateWindow(win_w, win_h, title, NULL, NULL);
    if (handle.window == NULL)
        exit(1);
    glfwMakeContextCurrent((GLFWwindow *)handle.window);
    glfwSwapInterval(1); // Enable vsync

    // Apparently glfw3 doesn't need a separately stored OpenGL context

    return handle;
}

void imgapp_destroyWindow(void *window, void *gl_context)
{
    glfwDestroyWindow((GLFWwindow *)window);
}

void imgapp_initWindowForOpenGL(void *window, void *gl_context)
{
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)window, true);
}

bool imgapp_pumpEvents(void *window)
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

    if (glfwWindowShouldClose((GLFWwindow *)window))
        return false;
    glfwPollEvents();
    return true;
}

void imgapp_newFrame(void * /* is glfw3 one-window only? */)
{
    ImGui_ImplGlfw_NewFrame();
}

void imgapp_presentFrame(void *window)
{
    glfwSwapBuffers((GLFWwindow *)window);
}

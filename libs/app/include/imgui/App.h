#pragma once

#include <functional>

#include <imgui.h>

class App
{
public:

    using RenderFunc = std::function<void()>;

    static void init();

    virtual ~App();

    auto openDefaultWindow(const char *title = "Dear ImGui SDL2+OpenGL3 example") -> App&;

    /* Returns true as long as the application is running, false if it is terminating. */
    bool pumpEvents(); 

    void updateAllWindows();

    auto onRender(RenderFunc) -> App&;

    const auto& clearColor() const { return clear_color; }
    void clearColor(const ImVec4& color) { clear_color = color; }

private:
    static const char *glsl_version;

    ImVec4 clear_color = ImVec4{ 0.45f, 0.55f, 0.60f, 1.00f };

    void *window = nullptr; // a void* should cover the needs of all platform libraries
    void * gl_context = nullptr;
    RenderFunc on_render;
};
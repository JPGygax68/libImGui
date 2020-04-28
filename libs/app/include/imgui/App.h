#pragma once

#include <functional>

#include <imgui.h>

class App
{
public:

    using RenderFunc = std::function<void()>;

    virtual ~App();

    void init();

    auto addFont(const char *filename, float size, const ImFontConfig* font_cfg_template = nullptr, 
        const ImWchar* glyph_ranges = nullptr) -> ImFont*;

    auto openDefaultWindow(const char *title = "libImGui default window") -> App&;

    void run();

    /* Returns true as long as the application is running, false if it is terminating. */
    bool pumpEvents(); 

    void updateAllWindows();

    auto onRender(RenderFunc) -> App&;

    const auto& clearColor() const { return clear_color; }
    void clearColor(const ImVec4& color) { clear_color = color; }

    // Queries

    auto dpiScaling() const { return dpi_scaling; }

private:

    const char *glsl_version = "#version 150"; // default

    float dpi_scaling = 1.0f;

    ImVec4 clear_color = ImVec4{ 0.45f, 0.55f, 0.60f, 1.00f };

    void *window = nullptr; // a void* should cover the needs of all platform libraries
    void * gl_context = nullptr;
    RenderFunc on_render;
};
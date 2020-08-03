#ifdef WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif
#include <stdio.h>
#include <chrono>
#include <cassert>
#include <filesystem>

#include <imgui/App.h>
#include <imgui/app_glue.h>


// PRIVATE TYPES

using myclock_t = std::chrono::high_resolution_clock;
using timepoint_t = std::chrono::time_point<myclock_t>;

// PRIVATE VARIABLES

static auto last_time = timepoint_t();


// App class implementation

void App::init()
{
    static bool init_done = false;

#if __APPLE__
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
#else
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
#endif

    if (!init_done)
    {
#ifdef WIN32
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        // io.FontGlobalScale = dpi_scaling;
        //io.DisplayFramebufferScale = ImVec2{ 0.5f, 0.5f };
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        imgapp_initPlatform();

        // TODO: support per-monitor DPI scaling?
        float dpi = imgapp_getMainMonitorDPI();
        dpi_scaling = dpi / 72.f; // >= 120 ? 1.5f : 1.0f;

        ImGui::GetStyle().ScaleAllSizes(dpi_scaling);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.txt' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        ImFont *font = addFont("Arial.ttf", 14.0f);
        //IM_ASSERT(font != NULL);

        // imgapp_initGraphicLibrary(glsl_version); // TODO: idiotic - only OpenGL has a GLSL version. Do better.

        atexit([]() {
            // Cleanup
            imgapp_deinitGraphicLibrary();  // TODO: provide a per-window deinitialization
            ImGui::DestroyContext();
            imgapp_deinitPlatform();
        });

        init_done = true;

    } // if !init_done
}

auto App::addFont(const char* filename, float size, const ImFontConfig* font_cfg_template,
    const ImWchar* glyph_ranges) -> ImFont*
{
#ifdef WIN32
    static const char *FONT_DIRECTORY = "c:\\Windows\\Fonts";
#else
#error PLATFORM NOT SUPPORTED YET
#endif

    auto path = std::filesystem::path(filename);
    if (path.is_relative()) path = std::filesystem::path(FONT_DIRECTORY) / path;

    auto& io = ImGui::GetIO();

    return io.Fonts->AddFontFromFileTTF(path.string().c_str(), round(dpiScaling() * size), font_cfg_template, glyph_ranges);
}

App::~App()
{
    // TODO: move to closeDefaultWindow()?
    imgapp_destroyWindow(_window, gl_context);
}

auto App::openDefaultWindow(const char *title) -> App &
{
    init();

    auto [display_width, display_height] = imgapp_getMainDisplayExtents();
    int win_w = display_width * 7 / 8, win_h = display_height * 7 / 8;

    auto [win, gl_ctx] = imgapp_openWindow(title, win_w, win_h);
    _window = win;
    gl_context = gl_ctx;

    imgapp_initGraphicLibrary("#version 150"); // TODO: get rid of GLSL version injection at this point
    
    return *this; // for "chaining"
}

void App::run()
{
    assert(_window != nullptr);

    while (pumpEvents())
    {
        updateAllWindows();
    }
}

bool App::pumpEvents()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.

    return imgapp_pumpEvents();
}

void App::updateAllWindows()
{
    using namespace std::chrono;

    // TODO: So far, there is only the Main window:

    // Compute delta t (future extension)
    // auto now = myclock_t::now();
    // auto &io = ImGui::GetIO();
    // io.DeltaTime = now == last_time ? 1.f / 60.f : duration<float>(now - last_time).count();
    // last_time = now;

    // Start the Dear ImGui frame
    imgapp_newFrame(_window);
    ImGui::NewFrame();

    imgapp_clearFrame(clear_color);

    int w, h;
    imgapp_getWindowContentSize(_window, &w, &h);

    on_render(w, h, _window);

    ImGui::EndFrame(); // not strictly necessary: ImGui::Render() does it automatically

    // Rendering
    ImGui::Render();

    imgapp_renderGui();

    if (after_render) after_render();

    // Present
    imgapp_presentFrame(_window);
}

auto App::onRender(RenderFunc func) -> App &
{
    assert(!on_render);

    on_render = func;

    return *this;
}

auto App::afterRender(AfterRenderFunc func) -> App&
{
    assert(!after_render);

    after_render = func;

    return *this;
}

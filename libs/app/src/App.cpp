#include <stdio.h>
#include <cassert>

#include <imgui/App.h>
#include <imgui/app_glue.h>

#ifdef LIBIMGUI_OPENGL3
#include "imgui_impl_opengl3.h"
#endif

const char *App::glsl_version = "#version 150"; // default

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
        atexit([]() {
            // Cleanup
            imgapp_deinitGraphicLibrary();
            imgapp_deinitPlatform();
            ImGui::DestroyContext();
        });

    } // if !init_done
}

App::~App()
{
    // TODO: move to closeDefaultWindow()?
    imgapp_destroyWindow(window, gl_context);
}

auto App::openDefaultWindow(const char *title) -> App &
{
    init();

    float dpi = imgapp_getMainMonitorDPI();
    auto [display_width, display_height] = imgapp_getMainDisplayExtents();
    float dpi_scaling = dpi / 72.f; // >= 120 ? 1.5f : 1.0f;
    int win_w = display_width * 7 / 8, win_h = display_height * 7 / 8;

    auto [win, gl_ctx] = imgapp_openWindow(title, win_w, win_h);
    window = win;
    gl_context = gl_ctx;

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        exit(1);
    }

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
    ImFont *font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", dpi_scaling * 14.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    return *this; // for "chaining"
}

void App::run()
{
    assert(window != nullptr);

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
    // TODO: So far, there is only the Main window:

    // Start the Dear ImGui frame
    // ImGui_ImplOpenGL3_NewFrame();
    imgapp_newFrame(window);
    ImGui::NewFrame();

    on_render();

    ImGui::EndFrame();

    // Rendering
    ImGui::Render();

    auto &io = ImGui::GetIO();
    imgapp_clearFrame(clear_color);
    imgapp_renderGui();

    // Present
    imgapp_presentFrame(window);
}

auto App::onRender(RenderFunc func) -> App &
{
    assert(!on_render);

    on_render = func;

    return *this;
}

#include <stdio.h>
#include <cassert>

#include <imgui/App.h>

#ifdef LIBIMGUI_SDL2
#include <SDL2/SDL.h>
#include "imgui_impl_sdl.h"
#elif LIBIMGUI_GLFW3
#include <GLFW/glfw3.h>
#include "imgui_impl_glfw.h"
#endif
#ifdef LIBIMGUI_OPENGL3
#include "imgui_impl_opengl3.h"
#endif

// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h> // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h> // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h> // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif


const char *App::glsl_version = nullptr;

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

    if (!init_done) {
#ifdef LIBIMGUI_SDL2
        // Setup SDL
        // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
        // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            exit(-1);
        }
        // Decide GL+GLSL versions
#if __APPLE__
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
#elif LIBIMGUI_GLFW3
        // Setup window
        glfwSetErrorCallback([](int error, const char *description) { fprintf(stderr, "Error %d: %s\n", error, description); });
        if (!glfwInit()) exit(1);

        // Decide GL+GLSL versions
#if __APPLE__
        // GL 3.2 + GLSL 150
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
#else
#error libImGuiApp: no platform library selected!
#endif

        atexit([]() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
#ifdef LIBIMGUI_SDL2
            ImGui_ImplSDL2_Shutdown();
#elif LIBIMGUI_GLFW
            ImGui_ImplGlfw_Shutdown();
#endif
            ImGui::DestroyContext();
        });

    } // if !init_done
}

    App::~App()
    {
        // TODO: move to closeDefaultWindow()?
#ifdef LIBIMGUI_SDL2
        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow((SDL_Window*)window);
        SDL_Quit();
#elif LIBIMGUI_GLFW3
        glfwDestroyWindow((GLFWwindow*)window);
        glfwTerminate();
#endif
    }

auto App::openDefaultWindow(const char* title) -> App&
{
#ifdef LIBIMGUI_SDL2
    // Query default monitor resolution
    float ddpi, hdpi, vdpi;
    if (SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi) != 0)
    {
        fprintf(stderr, "Failed to obtain DPI information for display 0: %s\n", SDL_GetError());
        exit(1);
    }
    float dpi_scaling = ddpi / 72.f; // >= 120 ? 1.5f : 1.0f;
    SDL_Rect display_bounds;
    if (SDL_GetDisplayUsableBounds(0, &display_bounds) != 0)
    {
        fprintf(stderr, "Failed to obtain bounds of display 0: %s\n", SDL_GetError());
        exit(1);
    }
    int win_w = display_bounds.w * 7 / 8, win_h = display_bounds.h * 7 / 8;
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, window_flags);
    gl_context = SDL_GL_CreateContext((SDL_Window*)window);
    SDL_GL_MakeCurrent((SDL_Window*)window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
#elif LIBIMGUI_GLFW3
    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int monitor_width, monitor_height;
    glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &monitor_width, &monitor_height);
    float hdpi = (float)mode->width  / ((float)monitor_width  / 25.4f);
    float vdpi = (float)mode->height / ((float)monitor_height / 25.4f);
    int win_w = mode->width * 7 / 8, win_h = mode->height * 7 / 8;
    float dpi_scaling = hdpi / 72.f;
    window = glfwCreateWindow(win_w, win_h, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        exit(1);
    glfwMakeContextCurrent((GLFWwindow*)window);
    glfwSwapInterval(1); // Enable vsync
#else
#error libImGuiApp: no platform library selected!
#endif

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

    // Setup Platform/Renderer bindings
#if LIBIMGUI_SDL2
    ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window, gl_context);
#elif LIBIMGUI_GLFW3
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window, true);
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

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
    while (pumpEvents()) {
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

#if LIBIMGUI_SDL2
    SDL_Event event;
    bool done = false;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID((SDL_Window*)window))
            done = true;
    }
    return !done;
#elif LIBIMGUI_GLFW3
    if (glfwWindowShouldClose((GLFWwindow*)window)) return false;
    glfwPollEvents();
    return true;
#endif
}

void App::updateAllWindows()
{
    // TODO: So far, there is only the Main window:

    auto& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
#if LIBIMGUI_SDL2
    ImGui_ImplSDL2_NewFrame((SDL_Window*)window);
#elif LIBIMGUI_GLFW3
    ImGui_ImplGlfw_NewFrame();
#endif
    ImGui::NewFrame();

    on_render();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Present
#if LIBIMGUI_SDL2
    SDL_GL_SwapWindow((SDL_Window*)window);
#elif LIBIMGUI_GLFW3
    glfwSwapBuffers((GLFWwindow*)window);
#endif
}

auto App::onRender(RenderFunc func) -> App&
{
    assert(!on_render);

    on_render = func;

    return *this;
}

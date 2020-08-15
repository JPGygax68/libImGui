#include <SDL2/SDL.h>
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_sdl.h>
#include <imgui/app_glue.h>

static auto openWindow(const char *title, int x, int y, int w, int h) -> WindowHandle
{
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    auto window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    auto window = SDL_CreateWindow(title, x, y, w, h, window_flags);
    auto gl_context = SDL_GL_CreateContext((SDL_Window*)window);
    SDL_GL_MakeCurrent((SDL_Window*)window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150"); // TODO: make version configurable (as in Dear ImGui example)

    return { (void*)window, (void*)gl_context };
}

auto imgapp_openWindow(const char *title, int win_w, int win_h) -> WindowHandle
{
    return openWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h);
}

auto imgapp_openWindowExt(const char* title, int x1, int y1, int x2, int y2) -> WindowHandle
{
    return openWindow(title, x1, y1, x2 - x1, y2 - y1);
}

void imgapp_newFrame(void *window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame((SDL_Window *)window);
}

void imgapp_getWindowContentSize(void *window, int *w, int *h)
{
    SDL_GetWindowSize((SDL_Window*)window, w, h);
}


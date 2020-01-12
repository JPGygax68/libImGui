#include <SDL2/SDL.h>
#include "app_glue.h"

auto imgapp_openWindow(const char *title, int win_w, int win_h) -> WindowHandle
{
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    void *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, window_flags);
    void *gl_context = SDL_GL_CreateContext((SDL_Window *)window);
    SDL_GL_MakeCurrent((SDL_Window *)window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Platform/Renderer bindings
    ImGui_ImplOpenGL3_Init(glsl_version);

    return {window, gl_context};
}

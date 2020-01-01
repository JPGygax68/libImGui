#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>
#include "imgui_impl_sdl.h"

#include "glue.h"


void app_init()
{
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
}

void app_shutdown()
{
    ImGui_ImplSDL2_Shutdown();
}

float app_getMainMonitorDPI()
{
    // Query default monitor resolution
    float ddpi, hdpi, vdpi;
    if (SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi) != 0)
    {
        fprintf(stderr, "Failed to obtain DPI information for display 0: %s\n", SDL_GetError());
        exit(1);
    }
    return ddpi; // we use the diagonal DPIs here
}

auto app_getMainDisplayExtents() -> Extents
{
    SDL_Rect bounds;
    if (SDL_GetDisplayUsableBounds(0, &bounds) != 0)
    {
        fprintf(stderr, "Failed to obtain bounds of display 0: %s\n", SDL_GetError());
        exit(1);
    }
    return { bounds.w, bounds.h };
}

auto app_openWindow(const char* title, int win_w, int win_h) -> WindowHandle
{
    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    void *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h, window_flags);
    void *gl_context = SDL_GL_CreateContext((SDL_Window*)window);
    SDL_GL_MakeCurrent((SDL_Window*)window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    return { window, gl_context };
}

void app_destroyWindow(void* window, void *gl_context)
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow((SDL_Window*)window);
    SDL_Quit(); // TODO: support multiple windows!
}

void app_initWindowForOpenGL(void *window, void *gl_context)
{
    ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window, gl_context);
}

bool app_pumpEvents(void *window)
{
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
}

void app_newFrame(void *window)
{
    ImGui_ImplSDL2_NewFrame((SDL_Window*)window);
}

void app_presentFrame(void* window)
{
    SDL_GL_SwapWindow((SDL_Window*)window);
}
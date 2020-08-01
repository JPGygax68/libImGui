#include <iostream>
#include <SDL2/SDL.h>
#include <imgui.h>
#include <examples/imgui_impl_sdl.h>

#include <imgui/app_glue.h>

void imgapp_initPlatform()
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

void imgapp_deinitPlatform()
{
    ImGui_ImplSDL2_Shutdown();
}

float imgapp_getMainMonitorDPI()
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

auto imgapp_getMainDisplayExtents() -> Extents
{
    SDL_Rect bounds;
    if (SDL_GetDisplayUsableBounds(0, &bounds) != 0)
    {
        fprintf(stderr, "Failed to obtain bounds of display 0: %s\n", SDL_GetError());
        exit(1);
    }
    return {bounds.w, bounds.h};
}

void imgapp_destroyWindow(void *window, void *gl_context)
{
    if (gl_context) SDL_GL_DeleteContext(gl_context);
    if (window) SDL_DestroyWindow((SDL_Window *)window);
    SDL_Quit(); // TODO: support multiple windows!
}

void imgapp_initWindowForOpenGL(void *window, void *gl_context)
{
    ImGui_ImplSDL2_InitForOpenGL((SDL_Window *)window, gl_context);
}

bool imgapp_pumpEvents()
{
    SDL_Event event;
    bool done = false;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        // TODO: keep a list of windows?
        // if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID((SDL_Window *)window))
        //    done = true;
    }
    return !done;
}

void imgapp_presentFrame(void *window)
{
    SDL_GL_SwapWindow((SDL_Window *)window);
}
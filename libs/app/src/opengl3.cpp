#include <imgui_impl_opengl3.h>
#include "app_glue.h"

void imgapp_initGraphicLibrary(const char *version)
{
}

void imgapp_deinitGraphicLibrary()
{
    ImGui_ImplOpenGL3_Shutdown();
}

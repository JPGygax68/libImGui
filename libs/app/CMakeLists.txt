project (libImGui_App)

# Application framework built on top of libImGUI (itself a packaging of Dear ImGui)

add_library(libImGui_App STATIC src/App.cpp include/imgui/App.h include/imgui/app_glue.h)
target_compile_features(libImGui_App PRIVATE cxx_std_17)
target_include_directories(libImGui_App PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
# TODO: install
# TODO: install public headers
if (NOT TARGET libImGui_Core)
  message("No Core target!")
endif()
target_link_libraries(libImGui_App PUBLIC libImGui_Core)

install(TARGETS libImGui_App EXPORT libImGui_App)
install(EXPORT libImGui_App DESTINATION "cmake")

# Adapters for platform/graphic library

add_library(libImGui_App_SDL_OpenGL3 STATIC src/sdl_opengl.cpp)
# TODO: macro definition to select OpenGL 3 ?
target_link_libraries(libImGui_App_SDL_OpenGL3 PRIVATE libImGui_App libImGui_SDL libImGui_OpenGL3)
# TODO: install

# TODO: more combinations

# Installation

install(TARGETS libImGui_App_SDL_OpenGL3 EXPORT libImGui_App_AdapterTargets)

install(EXPORT libImGui_App_AdapterTargets DESTINATION "cmake")

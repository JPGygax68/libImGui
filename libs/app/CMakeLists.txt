project (libImGui_App)

# Application framework built on top of libImGUI (itself a packaging of Dear ImGui)

add_library(libImGui_App STATIC src/App.cpp include/imgui/App.h include/imgui/app_glue.h)
target_compile_features(libImGui_App PRIVATE cxx_std_17)
target_include_directories(libImGui_App PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
# TODO: install
# TODO: install public headers
if (NOT TARGET libImGui::Core)
  message("No Core target!")
endif()
target_link_libraries(libImGui_App PUBLIC libImGui::Core)

set_target_properties(libImGui_App PROPERTIES EXPORT_NAME "App")
install(TARGETS libImGui_App EXPORT libImGui_App)
install(EXPORT libImGui_App NAMESPACE libImGui:: DESTINATION "cmake")

add_library(libImGui::App ALIAS libImGui_App) # mimick exported targets

# Adapters for platform/graphic library

add_library(libImGui_App_SDL_OpenGL3 STATIC src/sdl_opengl.cpp src/sdl.cpp src/opengl3.cpp)

# TODO: macro definition to select OpenGL 3 ?
target_link_libraries(libImGui_App_SDL_OpenGL3 PRIVATE libImGui::Glue libImGui_App libImGui_SDL libImGui_OpenGL3)
set_target_properties(libImGui_App_SDL_OpenGL3 PROPERTIES EXPORT_NAME "App_SDL_OpenGL3")
install(TARGETS libImGui_App_SDL_OpenGL3 EXPORT libImGui_App_SDL_OpenGL3)
# TODO? Use nested namespaces (check if libman supports that)
install(EXPORT libImGui_App_SDL_OpenGL3 NAMESPACE libImGui:: DESTINATION "cmake")

add_library(libImGui::App_SDL_OpenGL3 ALIAS libImGui_App_SDL_OpenGL3)

# TODO: more combinations


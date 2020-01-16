project(samples)

add_executable(hello hello.cpp)
set_property(TARGET hello PROPERTY VS_DPI_AWARE "PerMonitor")

target_link_libraries(hello PRIVATE libImGui::App libImGui::App_SDL_OpenGL3)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(hello PRIVATE SDL2::SDL2main GLEW::GLEW)

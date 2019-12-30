project(samples)

add_executable(hello hello.cpp)
set_property(TARGET hello PROPERTY VS_DPI_AWARE "PerMonitor")

if (0)
#find_package(glfw3 REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(hello PRIVATE libImGui SDL2::SDL2 SDL2::SDL2main GLEW::GLEW)
else()
target_link_libraries(hello PRIVATE libImGuiApp)
endif()

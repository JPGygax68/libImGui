project(samples)

add_executable(hello hello.cpp)
find_package(glfw3 REQUIRED)
#find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(hello PRIVATE libImGui SDL2:SDL2 GLEW::GLEW)

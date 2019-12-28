project(samples)

add_executable(hello hello.cpp)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
target_link_libraries(hello PRIVATE libImGui glfw3 GLEW::GLEW)

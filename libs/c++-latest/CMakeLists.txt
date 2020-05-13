project(libImGui_c++-latest)

add_library(
    ${PROJECT_NAME}
  STATIC
    include/imgui/imgui-c++-latest.h src/imgui-c++-latest.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(
    ${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/imgui>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC libImGui_Core)

add_library(libImGui::c++-latest ALIAS libImGui_c++-latest)

install(TARGETS libImGui_c++-latest EXPORT libImGui_c++-latest COMPONENT c++-latest PUBLIC_HEADER DESTINATION "include/imgui/")
install(EXPORT libImGui_c++-latest NAMESPACE libImGui:: COMPONENT c++-latest DESTINATION "cmake" FILE "libImGui_c++-latest-targets.cmake")


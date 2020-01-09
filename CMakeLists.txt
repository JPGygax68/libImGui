cmake_minimum_required(VERSION 3.14)

project(libImGui)

# Option: Platform Library

set(LIBIMGUI_PLATFORM_LIBRARY ${LIBIMGUI_PLATFORM_LIBRARY} CACHE STRING "Platform Library to link")
set_property(CACHE LIBIMGUI_PLATFORM_LIBRARY PROPERTY STRINGS SDL2 glfw3 Custom) # TODO: more...

# Define Library target

add_library(${PROJECT_NAME} INTERFACE)

# Add sources and headers from Dear ImGUI (injected into libImGui as a git submodule)

set(imgui_dir "${PROJECT_SOURCE_DIR}/external/imgui")

set(sources
  imgui.cpp
  imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp
  misc/cpp/imgui_stdlib.cpp

  # TODO: make graphics library configurable too
  examples/imgui_impl_opengl3.cpp
)
set(headers
  imconfig.h imgui.h imgui_internal.h
  imstb_rectpack.h imstb_textedit.h imstb_truetype.h
  # TODO: make this configurable
  examples/imgui_impl_glfw.h
  examples/imgui_impl_opengl3.h
)

# Dear ImGui sources and headers specific to the selected platform library

if (LIBIMGUI_PLATFORM_LIBRARY STREQUAL "SDL2")
  target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_PLATFORM_SDL2)
  # TODO: better way to handle targets with and without namespace?
  if (TARGET SDL2)
    message("using SDL2")
    target_link_libraries(${PROJECT_NAME} INTERFACE SDL2 SDL2main)
  else()
    find_package(SDL2 REQUIRED)
    target_link_libraries(${PROJECT_NAME} INTERFACE SDL2::SDL2 SDL2::SDL2main)
  endif()
  list(APPEND sources examples/imgui_impl_sdl.cpp)
  list(APPEND headers examples/imgui_impl_sdl.h)
elseif(LIBIMGUI_PLATFORM_LIBRARY STREQUAL "glfw3")
  target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_PLATFORM_GLFW3)
  target_compile_definitions(${PROJECT_NAME} INTERFACE GLFW_INCLUDE_NONE)
  find_package(glfw3 REQUIRED)
  target_link_libraries(${PROJECT_NAME} INTERFACE glfw)
  list(APPEND sources examples/imgui_impl_glfw.cpp)
  list(APPEND headers examples/imgui_impl_glfw.h)
else()
  target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_PLATFORM_CUSTOM)
  message(STATUS "libImGui: \"Custom\" platform selected -> user code must provide glue code!")
endif()

set(sources_args)
set(sources_fp)
foreach(src ${sources})
  list(APPEND sources_args $<BUILD_INTERFACE:${imgui_dir}/${src}> $<INSTALL_INTERFACE:src/${src}>)
  list(APPEND sources_fp ${imgui_dir}/${src})
endforeach()

set(headers_args)
foreach(hdr ${headers})
  list(APPEND headers_args $<BUILD_INTERFACE:${imgui_dir}/${hdr}> $<INSTALL_INTERFACE:include/${hdr}>)
  list(APPEND headers_fp ${imgui_dir}/${hdr})
endforeach()

target_sources(${PROJECT_NAME} INTERFACE ${sources_args} ${headers_args})
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/imgui> $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/imgui/examples> $<INSTALL_INTERFACE:include/examples>
)
if (LIBIMGUI_PLATFORM_LIBRARY STREQUAL "glfw3")
  target_compile_definitions(${PROJECT_NAME} INTERFACE GLFW_EXPOSE_NATIVE_WGL)
endif()

# OpenGL bindings
# TODO: there should be options for this too: 1) choose between OpenGL2,3, and Vulkan (possibly event DirectX);
# 2) for OpenGL (and Vulkan?), select a binding library

target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_OPENGL3)
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE GLEW::GLEW)
target_compile_definitions(${PROJECT_NAME} INTERFACE IMGUI_IMPL_OPENGL_LOADER_GLEW)

# Installation

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
  PUBLIC_HEADER DESTINATION "include/imgui" # TODO correct ?
)

cmake_policy(SET CMP0022 NEW)
install(EXPORT ${PROJECT_NAME}
  DESTINATION "cmake"
  EXPORT_LINK_INTERFACE_LIBRARIES
)

install(FILES ${sources_fp} ${headers_fp} DESTINATION "src/${PROJECT_NAME}/")

# Alias for build tree consumption

add_library(ImGui::Lib ALIAS ${PROJECT_NAME})

# TODO: support embedding fonts from misc/fonts using misc/binary_to_compressed_c.cpp

# TODO: support FreeType via misc/freetype

# TODO: support ImVector in the MSVC debugger via misc/natvis

# Components

add_subdirectory(libs/app)

add_subdirectory(samples)

cmake_minimum_required(VERSION 3.14)

project(libImGui)

add_library(${PROJECT_NAME} INTERFACE)

set(LIBIMGUI_PLATFORM_LIBRARY "SDL2" CACHE STRING "Platform Library to link")
set_property(CACHE LIBIMGUI_PLATFORM_LIBRARY PROPERTY STRINGS SDL2 glfw) # TODO: more...

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

# Add the source/header files supporting the chosen platform library
if (LIBIMGUI_PLATFORM_LIBRARY STREQUAL "SDL2")
  target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_SDL2)
  find_package(SDL2 REQUIRED)
  target_link_libraries(${PROJECT_NAME} INTERFACE SDL2::SDL2 SDL2::SDL2main)
  list(APPEND sources examples/imgui_impl_sdl.cpp)
  list(APPEND headers examples/imgui_impl_sdl.h)
elseif(LIBIMGUI_PLATFORM_LIBRARY STREQUAL "glfw")
  target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_GLFW3)
  target_compile_definitions(${PROJECT_NAME} INTERFACE GLFW_INCLUDE_NONE)
  find_package(glfw3 REQUIRED)
  target_link_libraries(${PROJECT_NAME} INTERFACE glfw)
  list(APPEND sources examples/imgui_impl_glfw.cpp)
  list(APPEND headers examples/imgui_impl_glfw.h)
endif()

# TODO: make this configurable too:
target_compile_definitions(${PROJECT_NAME} INTERFACE LIBIMGUI_OPENGL3)
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE GLEW::GLEW)
target_compile_definitions(${PROJECT_NAME} INTERFACE IMGUI_IMPL_OPENGL_LOADER_GLEW)

set(sources_args)
foreach(src ${sources})
  list(APPEND sources_args $<BUILD_INTERFACE:${imgui_dir}/${src}> $<INSTALL_INTERFACE:src/${src}>)
endforeach()

set(headers_args)
foreach(hdr ${headers})
  list(APPEND headers_args $<BUILD_INTERFACE:${imgui_dir}/${hdr}> $<INSTALL_INTERFACE:include/${hdr}>)
endforeach()
#message("sources: ${sources_args}")
#message("headers: ${headers_args}")

target_sources(${PROJECT_NAME} INTERFACE ${sources_args} ${headers_args})
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/imgui> $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/imgui/examples> $<INSTALL_INTERFACE:include/examples>
)
target_compile_definitions(${PROJECT_NAME} INTERFACE GLFW_EXPOSE_NATIVE_WGL)

# TODO: support embedding fonts from misc/fonts using misc/binary_to_compressed_c.cpp

# TODO: support FreeType via misc/freetype

# TODO: support ImVector in the MSVC debugger via misc/natvis

add_subdirectory(libs/app)

add_subdirectory(samples)
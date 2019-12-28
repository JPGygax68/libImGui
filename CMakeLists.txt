cmake_minimum_required(VERSION 3.14)

project(libImGui)

add_library(${PROJECT_NAME} INTERFACE)

set(imgui_dir "${PROJECT_SOURCE_DIR}/external/imgui")
set(sources
  imgui.cpp
  imgui_demo.cpp imgui_draw.cpp imgui_widgets.cpp
  misc/cpp/imgui_stdlib.cpp
  examples/imgui_impl_glfw.cpp
  examples/imgui_impl_opengl3.cpp
)
set(headers
  imconfig.h imgui.h imgui_internal.h
  imstb_rectpack.h imstb_textedit.h imstb_truetype.h
  # TODO: make this configurable
  examples/imgui_impl_glfw.h
  examples/imgui_impl_opengl3.h
)

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

add_subdirectory(samples)
cmake_minimum_required(VERSION 3.14)

project(libImGui)

# Add sources and headers from Dear ImGUI (injected into libImGui as a git submodule)

# Define library targets

set(root external/imgui)

add_library(libImGui_Core STATIC
  ${root}/imgui.cpp
  ${root}/imgui_draw.cpp ${root}/imgui_widgets.cpp
  ${root}/imgui.h ${root}/imconfig.h ${root}/imgui_internal.h
  ${root}/imstb_rectpack.h ${root}/imstb_textedit.h ${root}/imstb_truetype.h
  # The following are considered part of "Core" b/c they're not platform or GL dependent
  ${root}/misc/cpp/imgui_stdlib.cpp
  ${root}/imgui_demo.cpp
)

target_include_directories(libImGui_Core PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

install(
  TARGETS libImGui_Core
  EXPORT libImGui_CoreTargets
)

install(EXPORT libImGui_CoreTargets DESTINATION "cmake")

# Platform glue libs

# include_directories($<BUILD_INTERFACE:${root}/examples> $<INSTALL_INTERFACE:include>) # Another shortcut to specifying target include directories

# TODO: Detect which of them can be supported
# TODO: installation
# TODO: macro?

add_library(libImGui_Win32 STATIC
  ${root}/examples/imgui_impl_win32.cpp
  ${root}/examples/imgui_impl_win32.h
)
target_include_directories(libImGui_Win32 PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

add_library(libImGui_OSX STATIC
  ${root}/examples/imgui_impl_osx.mm
  ${root}/examples/imgui_impl_osx.h
)
target_include_directories(libImGui_OSX PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

add_library(libImGui_GLFW STATIC
  ${root}/examples/imgui_impl_glfw.cpp
  ${root}/examples/imgui_impl_glfw.h
)
target_include_directories(libImGui_GLFW PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)
target_compile_definitions(libImGui_GLFW INTERFACE GLFW_EXPOSE_NATIVE_WGL)

add_library(libImGui_Marmalade STATIC
  ${root}/examples/imgui_impl_marmalade.cpp
  ${root}/examples/imgui_impl_marmalade.h
)
target_include_directories(libImGui_Marmalade PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

add_library(libImGui_SDL STATIC
  ${root}/examples/imgui_impl_sdl.cpp
  ${root}/examples/imgui_impl_sdl.h
)
target_include_directories(libImGui_SDL PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

add_library(libImGui_Allegro5 STATIC
  ${root}/examples/imgui_impl_allegro5.cpp
  ${root}/examples/imgui_impl_allegro5.h
)
target_include_directories(libImGui_Allegro5 PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

install(
  TARGETS libImGui_SDL libImGui_Win32 libImGui_OSX libImGui_GLFW libImGui_Marmalade libImGui_Allegro5
  EXPORT libImGui_PlatformTargets
)

install(EXPORT libImGui_PlatformTargets DESTINATION "cmake")

# Graphics glue libs

set(includes PRIVATE ${root} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/examples> $<INSTALL_INTERFACE:include>)

add_library(libImGui_OpenGL2 STATIC
  ${root}/examples/imgui_impl_opengl2.cpp
  ${root}/examples/imgui_impl_opengl2.h
)
target_include_directories(libImGui_OpenGL2 ${includes})

add_library(libImGui_OpenGL3 STATIC
  ${root}/examples/imgui_impl_opengl3.cpp
  ${root}/examples/imgui_impl_opengl3.h
)
target_include_directories(libImGui_OpenGL3 ${includes})

add_library(libImGui_Vulkan STATIC
  ${root}/examples/imgui_impl_vulkan.cpp
  ${root}/examples/imgui_impl_vulkan.h
)
target_include_directories(libImGui_Vulkan ${includes})

add_library(libImGui_Metal STATIC
  ${root}/examples/imgui_impl_metal.mm
  ${root}/examples/imgui_impl_metal.h
)
target_include_directories(libImGui_Metal ${includes})

add_library(libImGui_DirectX9 STATIC
  ${root}/examples/imgui_impl_dx9.cpp
  ${root}/examples/imgui_impl_dx9.h
)
target_include_directories(libImGui_DirectX9  ${includes})

add_library(libImGui_DirectX10 STATIC
  ${root}/examples/imgui_impl_dx10.cpp
  ${root}/examples/imgui_impl_dx10.h
)
target_include_directories(libImGui_DirectX10 ${includes})

add_library(libImGui_DirectX11 STATIC
  ${root}/examples/imgui_impl_dx11.cpp
  ${root}/examples/imgui_impl_dx11.h
)
target_include_directories(libImGui_DirectX11 ${includes})

add_library(libImGui_DirectX12 STATIC
  ${root}/examples/imgui_impl_dx12.cpp
  ${root}/examples/imgui_impl_dx12.h
)
target_include_directories(libImGui_DirectX12 ${includes})

install(
  TARGETS
    libImGui_OpenGL2 libImGui_OpenGL3 libImGui_Vulkan
    libImGui_DirectX9 libImGui_DirectX10 libImGui_DirectX11 libImGui_DirectX12
    libImGui_Metal
  EXPORT libImGui_GLTargets
)

install(EXPORT libImGui_GLTargets DESTINATION "cmake")

# App component

add_subdirectory(libs/app)
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
#target_include_directories(libImGui_Core PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

install(TARGETS libImGui_Core EXPORT libImGui_Core)
install(EXPORT libImGui_Core DESTINATION "cmake")

# Glue code headers (header-only library)
# Only required if consumer needs to provide its own patform or graphic lib glue code

add_library(libImGui_Glue INTERFACE)
target_include_directories(libImGui_Glue INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/examples> $<INSTALL_INTERFACE:include>)

install(TARGETS libImGui_Glue EXPORT libImGui_Glue)
install(EXPORT libImGui_Glue DESTINATION "cmake")

# Dependencies helper function

function(add_dependencies_from_package target package dependencies)
  foreach(dep_tgt ${dependencies})
    if (NOT TARGET ${dep_tgt})
      find_package(${package} REQUIRED)
    endif()
    target_link_libraries(${target} PRIVATE ${dep_tgt})
  endforeach()
endfunction()

# Platform glue libs

# TODO: for dependencies, use libman package and library identifiers instead of CMake packages & targets
function(add_platform platform shortform default_enabled source_ext dep_pkg dep_targets)
  option(PLATFORM_LIBRARY_${platform} "Build module for platform ${platform}" ${default_enabled})
  if (${PLATFORM_LIBRARY_${platform}})
    set(target libImGui_${platform})
    add_library(${target} STATIC
      ${root}/examples/imgui_impl_${shortform}.${source_ext}
      ${root}/examples/imgui_impl_${shortform}.h
    )
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)
    add_dependencies_from_package(${target} "${dep_pkg}" "${dep_targets}")
    install(TARGETS ${target} EXPORT ${target})
    install(EXPORT ${target} DESTINATION "cmake")
  endif()
endfunction()

add_platform(Win32     win32     "${WIN32}" "cpp" "" "") # TODO: deps
add_platform(OSX       osx       "${APPLE}" "mm"  "" "") # TODO: deps
add_platform(glfw      glfw      FALSE      "cpp" "" "") # TODO: deps
add_platform(Marmalade marmalade FALSE      "cpp" "" "") # TODO: deps
add_platform(SDL       sdl       FALSE      "cpp" "SDL2" "SDL2::SDL2")
add_platform(Allegro5  allegro5  FALSE      "cpp" "" "") # TODO: deps

# Graphics glue libs

# TODO: for dependencies, use libman package and library identifiers instead of CMake packages & targets
function(add_graphiclib graphiclib shortform default_enabled source_ext dep_pkg dep_targets)
  option(GRAPHICS_${graphiclib} "Build module for graphic library ${graphiclib}" ${default_enabled})
  if (${GRAPHICS_${graphiclib}})
    set(target libImGui_${graphiclib})
    add_library(${target} STATIC
      ${root}/examples/imgui_impl_${shortform}.${source_ext}
      ${root}/examples/imgui_impl_${shortform}.h
    )
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)
    add_dependencies_from_package(${target} "${dep_pkg}" "${dep_targets}")
    install(TARGETS ${target} EXPORT ${target})
    install(EXPORT ${target} DESTINATION "cmake")
  endif()
endfunction()

add_graphiclib(OpenGL2   opengl2 TRUE       "cpp" "" "") # TODO: OpenGL bindings
add_graphiclib(OpenGL3   opengl3 TRUE       "cpp" "" "") # TODO: OpenGL bindings
add_graphiclib(Vulkan    vulkan  FALSE      "cpp" "" "") # TODO: autodetect presence
add_graphiclib(Metal     metal   FALSE      "mm"  "" "")
add_graphiclib(DirectX9  dx9     "${WIN32}" "cpp" "" "")
add_graphiclib(DirectX10 dx10    "${WIN32}" "cpp" "" "")
add_graphiclib(DirectX11 dx11    "${WIN32}" "cpp" "" "")
add_graphiclib(DirectX12 dx12    "${WIN32}" "cpp" "" "")

#set(includes PRIVATE ${root} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/examples> $<INSTALL_INTERFACE:include>)
#target_include_directories(libImGui_Vulkan ${includes})

# App component

add_subdirectory(libs/app)
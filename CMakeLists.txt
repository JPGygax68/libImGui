cmake_minimum_required(VERSION 3.16)

project(libImGui)

# Add sources and headers from Dear ImGUI (injected into libImGui as a git submodule)

# Define library targets

set(root external/imgui)

# "Core" library: consists of the Dear ImGui code itself

add_library(Core STATIC
  ${root}/imgui.cpp
  ${root}/imgui_draw.cpp ${root}/imgui_widgets.cpp
  ${root}/imgui.h ${root}/imconfig.h ${root}/imgui_internal.h
  ${root}/imstb_rectpack.h ${root}/imstb_textedit.h ${root}/imstb_truetype.h
  # The following are considered part of "Core" b/c they're not platform or GL dependent
  ${root}/misc/cpp/imgui_stdlib.cpp
  ${root}/imgui_demo.cpp
)

target_include_directories(Core PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)

# Make the Dear ImGui headers available using #include <imgui/imgui.h> etc.
set_target_properties(Core PROPERTIES PUBLIC_HEADER "${root}/imgui.h;${root}/imstb_rectpack.h;${root}/imstb_textedit.h;${root}/imstb_truetype.h")

add_library(libImGui::Core ALIAS Core)

# Install library file, headers, and cmake targets file
install(TARGETS Core EXPORT libImGui_Core COMPONENT Core)
install(TARGETS Core PUBLIC_HEADER DESTINATION "include/imgui/")
install(EXPORT libImGui_Core NAMESPACE libImGui:: COMPONENT Core DESTINATION "cmake" FILE "libImGui_Core-targets.cmake")

# "Glue" code headers (header-only library)

# Only required if consumer needs to provide its own platform and/or graphic lib glue code
# Note: this target does not add any dependencies (such as SDL, OpenGL etc.); consumers must obtain those themselves

add_library(Glue INTERFACE)
target_include_directories(Glue INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/examples> $<INSTALL_INTERFACE:include>)

add_library(libImGui::Glue ALIAS Glue)

install(TARGETS Glue EXPORT libImGui_Glue COMPONENT Glue)
install(EXPORT libImGui_Glue NAMESPACE libImGui:: DESTINATION "cmake" FILE "libImGui_Glue-targets.cmake")

# Create and install a CMake config file that imports both the Core and the Glue targets
install(CODE [[
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/libImGui-config.cmake
    "include(\${CMAKE_CURRENT_LIST_DIR}/libImGui_Core-targets.cmake)\n"
    "include(\${CMAKE_CURRENT_LIST_DIR}/libImGui_Glue-targets.cmake)\n"
  )
]])
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libImGui-config.cmake" DESTINATION "cmake/")

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
    set_target_properties(${target} PROPERTIES EXPORT_NAME ${platform})
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)
    add_dependencies_from_package(${target} "${dep_pkg}" "${dep_targets}")
    install(TARGETS ${target} EXPORT libImGui_Platforms)
  endif()
endfunction()

add_platform(Win32     win32     "${WIN32}" "cpp" "" "") # TODO: deps
add_platform(OSX       osx       "${APPLE}" "mm"  "" "") # TODO: deps
add_platform(glfw      glfw      FALSE      "cpp" "" "") # TODO: deps
add_platform(Marmalade marmalade FALSE      "cpp" "" "") # TODO: deps
add_platform(SDL       sdl       FALSE      "cpp" "SDL2" "SDL2::SDL2")
add_platform(Allegro5  allegro5  FALSE      "cpp" "" "") # TODO: deps

# TODO: this is stupid: the config file (which is not being created yet anyway) would have to satisfy the
# dependencies of *all* the platforms in order to be successfully imported - unacceptable.
# Each platform must therefore be given its own config file (TBD in the add_platform() function above).
install(EXPORT libImGui_Platforms NAMESPACE libImGui:: COMPONENT "Platforms" DESTINATION "cmake")

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
    set_target_properties(${target} PROPERTIES EXPORT_NAME ${graphiclib})
    target_include_directories(${target} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}> $<INSTALL_INTERFACE:include>)
    add_dependencies_from_package(${target} "${dep_pkg}" "${dep_targets}")
    install(TARGETS ${target} EXPORT libImGui_GraphicLibs)
  endif()
endfunction()

add_graphiclib(OpenGL2   opengl2 TRUE       "cpp" "" "") # TODO: OpenGL bindings
add_graphiclib(OpenGL3   opengl3 TRUE       "cpp" "" "") # TODO: OpenGL bindings
add_graphiclib(Vulkan    vulkan  FALSE      "cpp" "" "") # TODO: autodetect presence
add_graphiclib(Metal     metal   FALSE      "mm"  "" "")
add_graphiclib(DirectX9  dx9     "${WIN32}" "cpp" "" "")
add_graphiclib(DirectX10 dx10    "${WIN32}" "cpp" "" "")
add_graphiclib(DirectX11 dx11    "${WIN32}" "cpp" "" "")

if (${WIN32} AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
  add_graphiclib(DirectX12 dx12 "${dx12}" "cpp" "" "")
endif()

install(EXPORT libImGui_GraphicLibs NAMESPACE libImGui:: COMPONENT "GraphicLibs" DESTINATION "cmake")

#set(includes PRIVATE ${root} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/examples> $<INSTALL_INTERFACE:include>)
#target_include_directories(libImGui_Vulkan ${includes})

# App component

add_subdirectory(libs/app)

# Samples

add_subdirectory(samples EXCLUDE_FROM_ALL)
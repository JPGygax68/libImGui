cmake_minimum_required(VERSION 3.16)

project(libImGui)

# Add sources and headers from Dear ImGUI (injected into libImGui as a git submodule)

# Define library targets

set(root external/imgui)

# "Core" library: consists of the Dear ImGui code itself

add_library(libImGui_Core STATIC
  ${root}/imgui.cpp
  ${root}/imgui_draw.cpp ${root}/imgui_widgets.cpp
  ${root}/imgui.h ${root}/imconfig.h ${root}/imgui_internal.h
  ${root}/imstb_rectpack.h ${root}/imstb_textedit.h ${root}/imstb_truetype.h
  # The following are considered part of "Core" b/c they're not platform or GL dependent
  ${root}/misc/cpp/imgui_stdlib.cpp
  ${root}/imgui_demo.cpp
)

target_include_directories(libImGui_Core
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external/imgui>
    $<INSTALL_INTERFACE:include>
)

# Make the Dear ImGui headers available using #include <imgui/imgui.h> etc.
set_target_properties(libImGui_Core PROPERTIES PUBLIC_HEADER "${root}/imgui.h;${root}/imconfig.h;${root}/imstb_rectpack.h;${root}/imstb_textedit.h;${root}/imstb_truetype.h")

add_library(libImGui::Core ALIAS libImGui_Core)

# Install library file, headers, and cmake targets file
install(TARGETS libImGui_Core EXPORT libImGui_Core COMPONENT Core PUBLIC_HEADER DESTINATION "include/imgui/")
install(EXPORT libImGui_Core NAMESPACE libImGui:: COMPONENT Core DESTINATION "cmake" FILE "libImGui_Core-targets.cmake")

# Dependencies helper function (for platform and graphics libs)

function(add_dependencies_from_package target package dependencies)
  foreach(dep_tgt ${dependencies})
    if (NOT TARGET ${dep_tgt})
      find_package(${package} REQUIRED)
    endif()
    target_link_libraries(${target} PRIVATE ${dep_tgt})
  endforeach()
endfunction()

function(add_component comp_type comp shortform default_enabled source_ext dep_pkg dep_targets)
  string(TOUPPER "${comp_type}" comp_type_uc)
  option(${comp_type_uc}_LIBRARY_${comp} "Build module for ${platform}" ${default_enabled})
  if (${comp_type_uc}_LIBRARY_${comp})
    set(target libImGui_${comp})
    add_library(${target} STATIC
      ${root}/examples/imgui_impl_${shortform}.${source_ext}
      ${root}/examples/imgui_impl_${shortform}.h
    )
    set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${root}/examples/imgui_impl_${shortform}.h")
    set_target_properties(${target} PROPERTIES EXPORT_NAME ${comp})
    target_include_directories(${target} PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/> $<INSTALL_INTERFACE:include/imgui>
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/${root}/examples> $<INSTALL_INTERFACE:include/imgui/examples/>
    )
    add_dependencies_from_package(${target} "${dep_pkg}" "${dep_targets}")
    add_library(libImGui::${comp} ALIAS libImGui_${comp})
    #install(TARGETS ${target} PUBLIC_HEADER DESTINATION "include/imgui/examples/")
    install(TARGETS ${target} EXPORT ${target} PUBLIC_HEADER DESTINATION "include/imgui/examples/")
    install(EXPORT ${target} NAMESPACE libImGui:: DESTINATION "cmake" FILE "libImGui_${comp}-targets.cmake")
    install(CODE "\
      file(WRITE \"${CMAKE_CURRENT_BINARY_DIR}/libImGui_${comp}-config.cmake\"\n\
        \"include(\\\"\\$\{CMAKE_CURRENT_LIST_DIR\}/libImGui_${comp}-targets.cmake\\\")\\\"\n\
      )\
    ")
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libImGui_${comp}-config.cmake" DESTINATION "cmake/")
  endif()
endfunction()

# Platform libs

# TODO: for dependencies, use libman package and library identifiers instead of CMake packages & targets
function(add_platform platform shortform default_enabled source_ext dep_pkg dep_targets)
  add_component("platform" "${platform}" "${shortform}" "${default_enabled}" "${source_ext}" "${dep_pkg}" "${dep_targets}")
endfunction()

add_platform(Win32     win32     "${WIN32}" "cpp" "" "") # TODO: deps
add_platform(OSX       osx       "${APPLE}" "mm"  "" "") # TODO: deps
add_platform(glfw      glfw      FALSE      "cpp" "" "") # TODO: deps
add_platform(Marmalade marmalade FALSE      "cpp" "" "") # TODO: deps
add_platform(SDL       sdl       FALSE      "cpp" "SDL2" "SDL2::SDL2")
add_platform(Allegro5  allegro5  FALSE      "cpp" "" "") # TODO: deps

# Graphics libs

function(add_graphiclib lib shortform default_enabled source_ext dep_pkg dep_targets)
  add_component("graphics" "${lib}" "${shortform}" "${default_enabled}" "${source_ext}" "${dep_pkg}" "${dep_targets}")
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

# Extra libraries

add_subdirectory(libs/c++-latest)

# App component

add_subdirectory(libs/app)

# Samples

add_subdirectory(samples EXCLUDE_FROM_ALL)
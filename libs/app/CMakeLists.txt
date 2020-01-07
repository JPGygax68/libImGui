project (libImGuiApp)

# Application framework built on top of libImGUI (itself a packaging of Dear ImGui)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE libImGui)

if (LIBIMGUI_PLATFORM_LIBRARY STREQUAL "SDL2")
  set(glue_sources "src/glue_sdl2.cpp")
elseif (LIBIMGUI_PLATFORM_LIBRARY STREQUAL "glfw3")
  set(glue_sources "src/glue_glfw3.cpp")
elseif (LIBIMGUI_PLATFORM_LIBRARY STREQUAL "Custom")
  # Nothing to do here
else()
  message(FATAL_ERROR "No platform library selected")
endif()

#set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17) # not allowed on interface libraries? why?

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

set(sources App.cpp)
set(headers)
set(public_headers App.h app/glue.h)

set(sources_args)
set(sources_fp)
foreach(src ${sources} ${headers})
  set(source_fp "${PROJECT_SOURCE_DIR}/src/${src}")
  list(APPEND sources_fp ${source_fp})
  list(APPEND sources_args $<BUILD_INTERFACE:${source_fp}> $<INSTALL_INTERFACE:src/imgui/${src}>)
endforeach()
foreach(phdr ${public_headers})
  list(APPEND sources_args $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/imgui/${phdr}> $<INSTALL_INTERFACE:include/imgui/${phdr}>)
endforeach()

target_sources(${PROJECT_NAME} INTERFACE ${sources_args})

target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME} EXPORT libImGui
  PUBLIC_HEADER DESTINATION "include/imgui" # TODO probably doing nothing
)

install(FILES ${sources_fp} DESTINATION "src/${PROJECT_NAME}/")


# Alias target for convention
add_library(ImGui::App ALIAS ${PROJECT_NAME})

# TODO: installation
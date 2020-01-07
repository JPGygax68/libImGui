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

target_sources(${PROJECT_NAME} INTERFACE src/App.cpp include/imgui/App.h include/imgui/app/glue.h ${glue_sources})
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
)

# Alias target for convention
add_library(ImGui::App ALIAS ${PROJECT_NAME})

# TODO: installation
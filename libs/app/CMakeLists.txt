project (libImGuiApp)

# Application framework built on top of libImGUI (itself a packaging of Dear ImGui)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE libImGui)

target_sources(${PROJECT_NAME} INTERFACE src/App.cpp include/imgui/App.h)
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
)

# TODO: installation
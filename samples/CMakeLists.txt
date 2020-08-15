project(samples)

add_executable(hello hello.cpp)
set_property(TARGET hello PROPERTY VS_DPI_AWARE "PerMonitor")
set_target_properties(hello PROPERTIES VS_JUST_MY_CODE_DEBUGGING ON)

if (TARGET libImGui::App_SDL_OpenGL3)
  target_link_libraries(hello PRIVATE libImGui::App_SDL_OpenGL3)
else()
  message(FATAL_ERROR "Cannot build \"Hello\" sample because no supported glue component comination (e.g. SDL+OpenGL3) has been built")
endif()

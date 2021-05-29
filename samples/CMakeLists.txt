project(samples)

add_executable(hello hello.cpp)
set_property(TARGET hello PROPERTY VS_DPI_AWARE "PerMonitor")
set_target_properties(hello PROPERTIES VS_JUST_MY_CODE_DEBUGGING ON)
target_compile_features(hello PRIVATE cxx_std_20)

if (TARGET libImGui::App_SDL_OpenGL3)
  target_link_libraries(hello PRIVATE libImGui::App_SDL_OpenGL3)
else()
  message(WARNING "Cannot build \"Hello\" sample because no supported glue component combination (e.g. SDL+OpenGL3) has been built")
endif()

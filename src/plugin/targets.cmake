add_library(plugin-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${SRC}/plugin/main/Register.cpp
)
target_compile_definitions(plugin-objs PRIVATE
  BUILD_PLUGIN
)
target_include_directories(plugin-objs PRIVATE
  ${SRC}/interfacelib/public
)

add_library(plugin-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${SRC}/plugin/test/PluginTest.cpp
)
target_include_directories(plugin-objs PRIVATE
  ${SRC}/interfacelib/public
)

add_library(plugin SHARED
  $<TARGET_OBJECTS:plugin-objs>
)

set_target_properties(
  plugin-objs
  plugin-test-objs
  plugin
  PROPERTIES FOLDER plugin
)

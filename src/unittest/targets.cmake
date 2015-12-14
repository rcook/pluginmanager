add_executable(unittest
  $<TARGET_OBJECTS:app-objs>
  $<TARGET_OBJECTS:app-test-objs>
  $<TARGET_OBJECTS:interfacelib-objs>
  $<TARGET_OBJECTS:interfacelib-test-objs>
  $<TARGET_OBJECTS:plugin-objs>
  $<TARGET_OBJECTS:plugin-test-objs>
  $<TARGET_OBJECTS:utillib-objs>
  $<TARGET_OBJECTS:utillib-test-objs>
  ${SRC}/unittest/main/main.cpp
)
if(NOT NO_USE_CATCH_MAIN)
  add_target_definitions(unittest
    USE_CATCH_MAIN
  )
endif()
target_include_directories(unittest PRIVATE
  ${SRC}/app/test
  ${SRC}/interfacelib/test
  ${SRC}/plugin/test
  ${SRC}/utillib/test
)
target_link_libraries(unittest
  ${CMAKE_DL_LIBS}
)

set_target_properties(
  unittest
  PROPERTIES FOLDER unittest
)

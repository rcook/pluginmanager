add_library(interfacelib-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${SRC}/interfacelib/main/Registrar.cpp
  ${SRC}/interfacelib/public/interfacelib/OpFunc.h
  ${SRC}/interfacelib/public/interfacelib/Registrar.h
  ${SRC}/interfacelib/public/interfacelib/defs.h
)
target_compile_definitions(interfacelib-objs PRIVATE
  BUILD_INTERFACELIB
)
target_include_directories(interfacelib-objs PRIVATE
  ${SRC}/interfacelib/public
)

add_library(interfacelib-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${SRC}/interfacelib/test/OpFuncTest.cpp
  ${SRC}/interfacelib/test/RegistrarTest.cpp
)
target_compile_definitions(interfacelib-test-objs PRIVATE
  BUILD_INTERFACELIB
)
target_include_directories(interfacelib-test-objs PRIVATE
  ${SRC}/interfacelib/public
)

add_library(interfacelib SHARED
  $<TARGET_OBJECTS:interfacelib-objs>
)

set_target_properties(
  interfacelib-objs
  interfacelib-test-objs
  interfacelib
  PROPERTIES FOLDER interfacelib
)

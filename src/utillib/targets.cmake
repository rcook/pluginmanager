add_library(utillib-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${SRC}/utillib/main/OSError.cpp
  ${SRC}/utillib/main/fs.cpp
  ${SRC}/utillib/main/string.cpp
  ${SRC}/utillib/public/utillib/OSError.h
  ${SRC}/utillib/public/utillib/ScopedHandle.h
  ${SRC}/utillib/public/utillib/Trie.h
  ${SRC}/utillib/public/utillib/fs.h
  ${SRC}/utillib/public/utillib/string.h
  ${SRC}/utillib/public/utillib/validate.h
)
target_include_directories(utillib-objs PRIVATE
  ${SRC}/utillib/public
)

add_library(utillib-test-objs OBJECT
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  ${SRC}/utillib/test/ScopedHandleTest.cpp
  ${SRC}/utillib/test/StringTest.cpp
  ${SRC}/utillib/test/TrieTest.cpp
)
target_include_directories(utillib-test-objs PRIVATE
  ${SRC}/utillib/public
)

add_library(utillib STATIC
  $<TARGET_OBJECTS:utillib-objs>
)

set_target_properties(
  utillib-objs
  utillib-test-objs
  utillib
  PROPERTIES FOLDER utillib
)

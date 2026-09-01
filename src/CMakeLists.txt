include_directories(include)

add_subdirectory(storage)

add_library(
  ts_ext_library OBJECT
  ts_extension.cpp
  ts_storage.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:ts_ext_library>
    PARENT_SCOPE)

add_library(
  ts_ext_storage OBJECT
  ts_catalog.cpp
  ts_schema_entry.cpp
  ts_table_entry.cpp
  ts_transaction_manager.cpp
  ts_transaction.cpp
  ts_create_table_as.cpp
  ts_insert.cpp
  ts_delete.cpp
  ts_update.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:ts_ext_storage>
    PARENT_SCOPE)


if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/../../third_party/sqlite/sqlite3.h")
message("Found sqlite source code -> compile sqlite-log driver with static sqlite library")

include_directories(
   .
   ${CMAKE_SOURCE_DIR}/third_party/sqlite
)

set(server_example_SRCS
   server_example_logging.c
   static_model.c
   ${CMAKE_CURRENT_LIST_DIR}/../../src/logging/drivers/sqlite/log_storage_sqlite.c
)

set(sqlite_SRCS
   ${CMAKE_CURRENT_LIST_DIR}/../../third_party/sqlite/sqlite3.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION")

IF(MSVC)
set_source_files_properties(${server_example_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(server_example_logging
  ${server_example_SRCS}
  ${sqlite_SRCS}
)

target_link_libraries(server_example_logging
    iec61850
)

ELSE()

message("server-example-logging: sqlite not found")

ENDIF()


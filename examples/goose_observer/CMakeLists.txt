
set(goose_observer_SRCS
   goose_observer.c
)

IF(WIN32)

set_source_files_properties(${goose_observer_SRCS}
                                       PROPERTIES LANGUAGE CXX)
add_executable(goose_observer
  ${goose_observer_SRCS}
)

target_link_libraries(goose_observer
    iec61850
)

ELSE(WIN32)

add_executable(goose_observer
  ${goose_observer_SRCS}
)

target_link_libraries(goose_observer
    iec61850
)

ENDIF(WIN32)




set(goose_observer_SRCS
   goose_observer.c
)

IF(MSVC)

set_source_files_properties(${goose_observer_SRCS}
                                       PROPERTIES LANGUAGE CXX)
                                       
ENDIF(MSVC)

add_executable(goose_observer
  ${goose_observer_SRCS}
)

target_link_libraries(goose_observer
    iec61850
)



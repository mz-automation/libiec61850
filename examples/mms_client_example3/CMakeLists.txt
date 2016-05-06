
set(mms_client_example3_SRCS
   mms_client_example3.c
)

IF(MSVC)
set_source_files_properties(${mms_client_example3_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(mms_client_example3
  ${mms_client_example3_SRCS}
)

target_link_libraries(mms_client_example3
    iec61850
)

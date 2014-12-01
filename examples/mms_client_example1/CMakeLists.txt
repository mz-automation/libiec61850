
set(mms_client_example1_SRCS
   mms_client_example1.c
)

IF(WIN32)
set_source_files_properties(${mms_client_example1_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(mms_client_example1
  ${mms_client_example1_SRCS}
)

target_link_libraries(mms_client_example1
    iec61850
)

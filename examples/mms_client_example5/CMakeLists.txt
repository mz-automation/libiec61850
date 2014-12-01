
set(mms_client_example5_SRCS
   mms_client_example5.c
)

IF(WIN32)
set_source_files_properties(${mms_client_example5_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(mms_client_example5
  ${mms_client_example5_SRCS}
)

target_link_libraries(mms_client_example5
    iec61850
)

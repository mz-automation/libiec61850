
set(mms_client_example2_SRCS
   mms_client_example2.c
)

IF(WIN32)
set_source_files_properties(${mms_client_example2_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(mms_client_example2
  ${mms_client_example2_SRCS}
)

target_link_libraries(mms_client_example2
    iec61850
)


set(mms_client_example2_SRCS
   mms_client_example2.c
)

IF(MSVC)
set_source_files_properties(${mms_client_example2_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(mms_client_example2
  ${mms_client_example2_SRCS}
)

target_link_libraries(mms_client_example2
    iec61850
)

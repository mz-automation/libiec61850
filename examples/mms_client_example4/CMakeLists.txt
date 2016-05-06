
set(mms_client_example4_SRCS
   mms_client_example4.c
)

IF(MSVC)
set_source_files_properties(${mms_client_example4_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)

add_executable(mms_client_example4
  ${mms_client_example4_SRCS}
)

target_link_libraries(mms_client_example4
    iec61850
)

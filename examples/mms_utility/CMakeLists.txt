
set(mms_utility_SRCS
   mms_utility.c
)

IF(WIN32)
set_source_files_properties(${mms_utility_SRCS}
                                       PROPERTIES LANGUAGE CXX)
ENDIF(WIN32)

add_executable(mms_utility
  ${mms_utility_SRCS}
)

target_link_libraries(mms_utility
    iec61850
)

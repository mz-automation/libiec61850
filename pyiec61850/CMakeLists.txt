cmake_minimum_required(VERSION 3.12)

cmake_policy(SET CMP0078 NEW)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14.0")
    cmake_policy(SET CMP0086 NEW)
endif()

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(Python COMPONENTS Interpreter Development REQUIRED)

include_directories(${Python_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_SWIG_FLAGS "")
set_property(SOURCE iec61850.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE iec61850.i PROPERTY SWIG_MODULE_NAME pyiec61850)

if(WIN32)
    set(LIBS iec61850 ws2_32)
else()
    set(LIBS iec61850-shared)
endif()

swig_add_library(pyiec61850
    LANGUAGE python
    SOURCES iec61850.i
)

swig_link_libraries(pyiec61850 ${LIBS})

# Finding python modules install path
execute_process(
	COMMAND ${Python_EXECUTABLE} -c
	"from sysconfig import get_path; import sys; sys.stdout.write(get_path('platlib'))"
	OUTPUT_VARIABLE PYTHON_SITE_DIR
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pyiec61850.py DESTINATION ${PYTHON_SITE_DIR})
install(TARGETS pyiec61850 LIBRARY DESTINATION ${PYTHON_SITE_DIR})

add_test(test_pyiec61850 ${Python_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/test_pyiec61850.py)

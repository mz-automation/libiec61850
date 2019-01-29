# The SWIG functions/macros used in this module, swig_add_module and swig_add_library
# are not available in CMake versions earlier than 3.8
# cmake_minimum_required(VERSION 3.8)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonInterp ${BUILD_PYTHON_VERSION} REQUIRED)
find_package(PythonLibs ${PYTHON_VERSION_STRING} EXACT REQUIRED)

include_directories(${PYTHON_INCLUDE_PATH})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_SWIG_FLAGS "")
set_property(SOURCE iec61850.i PROPERTY CPLUSPLUS ON)

if(WIN32)
    set(LIBS iec61850 ws2_32)
else()
    set(LIBS iec61850-shared)
endif()

if(${CMAKE_VERSION} VERSION_LESS 3.8)
    swig_add_module(iec61850 python iec61850.i)
else()
    swig_add_library(iec61850
        LANGUAGE python
        SOURCES iec61850.i
    )
endif()

swig_link_libraries(iec61850 ${PYTHON_LIBRARIES} ${LIBS})

# Finding python modules install path
execute_process(
	COMMAND ${PYTHON_EXECUTABLE} -c
	"from distutils.sysconfig import get_python_lib; import sys; sys.stdout.write(get_python_lib())"
	OUTPUT_VARIABLE PYTHON_SITE_DIR
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/iec61850.py DESTINATION ${PYTHON_SITE_DIR})
install(TARGETS _iec61850 LIBRARY DESTINATION ${PYTHON_SITE_DIR})

add_test(test_pyiec61850 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/test_pyiec61850.py)

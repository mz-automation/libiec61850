# The SWIG functions/macros used in this module, swig_add_module and swig_add_library
# are not available in CMake versions earlier than 3.8
# cmake_minimum_required(VERSION 3.8)

# We use explicitly passed PYTHON_INCLUDE_DIRS/PYTHON_EXECUTABLE because find_package finds the wrong python dist
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

include_directories(${PYTHON_INCLUDE_DIRS})
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

swig_link_libraries(iec61850 ${LIBS})  # Note: not linking against python https://github.com/pypa/cibuildwheel/issues/727#issuecomment-866278052
add_test(test_pyiec61850 ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/test_pyiec61850.py)

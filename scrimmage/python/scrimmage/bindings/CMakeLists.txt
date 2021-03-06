#--------------------------------------------------------
# Library Creation
#--------------------------------------------------------
set(LIBRARY_NAME "py_bindings")

include_directories(include)
add_library(py_math SHARED src/py_math.cpp)

add_library(py_network SHARED src/py_network.cpp)
target_link_libraries(py_network scrimmage)

add_library(py_common SHARED src/py_common.cpp)
target_link_libraries(py_common scrimmage)

add_library(py_autonomy SHARED src/py_autonomy.cpp)
target_link_libraries(py_autonomy scrimmage)

add_library(py_utils SHARED src/py_utils.cpp)
target_link_libraries(py_utils scrimmage)
if (BUILD_GRPC)
    add_dependencies(py_utils grpc)
endif()

pybind11_add_module(${LIBRARY_NAME} src/py_bindings.cpp)

target_link_libraries(${LIBRARY_NAME} PRIVATE py_math py_network py_common py_autonomy py_utils)

# Push up the SCRIMMAGE_LIBS variable
set_target_properties(${LIBRARY_NAME} PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      OUTPUT_NAME py_bindings)

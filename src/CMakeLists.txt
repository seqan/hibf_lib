cmake_minimum_required (VERSION 3.8)

add_library (chopper_interface INTERFACE)
target_link_libraries (chopper_interface INTERFACE seqan3::seqan3)
target_include_directories (chopper_interface INTERFACE ../include)
target_include_directories (chopper_interface INTERFACE ../lib/lemon/include)
target_include_directories (chopper_interface INTERFACE ../lib/robin-hood-hashing/src/include)
target_compile_definitions (chopper_interface INTERFACE ${SEQAN_DEFINITIONS})
target_compile_options (chopper_interface INTERFACE ${SEQAN_CXX_FLAGS_LIST})
target_link_libraries (chopper_interface INTERFACE "${SEQAN_LIBRARIES}")
target_include_directories (chopper_interface INTERFACE "${SEQAN_INCLUDE_DIRS}")

add_library (chopper_build_lib STATIC chopper_build.cpp)
target_link_libraries (chopper_build_lib "chopper_interface")

add_library (chopper_search_lib STATIC chopper_search.cpp)
target_link_libraries (chopper_search_lib "chopper_interface")

add_library (chopper_lib INTERFACE)
target_link_libraries (chopper_lib INTERFACE "chopper_interface" "chopper_build_lib" "chopper_search_lib")

add_executable (chopper chopper.cpp)
target_link_libraries (chopper "chopper_lib")

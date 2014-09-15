project(hiredis C)
cmake_minimum_required(VERSION 2.8.6)

set(HIREDIS_VERSION_MAJOR 0)
set(HIREDIS_VERSION_MINOR 11)
set(HIREDIS_VERSION_STRING "${HIREDIS_VERSION_MAJOR}.${HIREDIS_VERSION_MINOR}")

set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_definitions(-D_WIN32_WINNT=0x0600)

# Library
include_directories("${PROJECT_SOURCE_DIR}")
set(SRC_FILES "async.c" "dict.c" "hiredis.c" "net.c" "sds.c" "portability.c")
set(INCLUDE_FILES "async.h" "dict.h" "fmacros.h" "hiredis.h" "net.h" "sds.h")

add_library(${PROJECT_NAME}  ${SRC_FILES} ${INCLUDE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  VERSION ${HIREDIS_VERSION_STRING}
  SOVERSION ${HIREDIS_VERSION_STRING}
  C_STANDART 99
)
#add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)
#add_library(${PROJECT_NAME}-shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)

macro(append_compile_flags target_name flags)
  set_property(TARGET ${target_name} APPEND_STRING PROPERTY COMPILE_FLAGS " ${flags}")
  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flags}")
endmacro()

if(NOT DEFINED CMAKE_C_STANDARD)
  if((CMAKE_C_COMPILER_ID MATCHES "Clang") OR
     (CMAKE_C_COMPILER_ID MATCHES "GNU"))
    append_compile_flags(${PROJECT_NAME} "-std=c99")
  endif()
endif()

if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
  append_compile_flags(${PROJECT_NAME} "/Wall")
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  append_compile_flags(${PROJECT_NAME} "-Weverything")
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU")
  append_compile_flags(${PROJECT_NAME} "-Wall -Wextra -pedantic")
endif()

if(WIN32)
  # Add windows related network library dependencies
  target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
else()
  if(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} nsl socket)
  endif()
  append_compile_flags(${PROJECT_NAME} "-fPIC")
endif()


macro(build_additional additional_name additional_src_list)
#  add_executable(${additional_name} EXCLUDE_FROM_ALL ${additional_src_list})
  add_executable(${additional_name} ${additional_src_list})
  target_link_libraries("${additional_name}" "${PROJECT_NAME}")
#  add_dependency("${additional_name}" "${PROJECT_NAME}-static")
endmacro()


# Building examples
add_custom_target(examples)
include_directories("${PROJECT_SOURCE_DIR}/adapters")
# Do this until the other examples have not been ported to Windows
if(WIN32)
  build_additional(${PROJECT_NAME}-example "examples/example.c")
  add_dependencies(examples ${PROJECT_NAME}-example)
else()
  file(GLOB examples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} examples/*)
  foreach(example_source ${examples})
    get_filename_component(test_name "${example_source}" NAME_WE)
    build_additional(${PROJECT_NAME}-${example_name} "${example_source}")
    add_dependencies(examples ${PROJECT_NAME}-${example_name})
  endforeach()
endif()

# Testing
if(BUILD_TESTING)
  build_additional(${PROJECT_NAME}-test "test.c")

  enable_testing()

  add_test(NAME ${PROJECT_NAME}-test COMMAND "${PROJECT_NAME}-test ")
endif()

# Installing
if(MINGW)
  get_filename_component(MINGW_PATH ${CMAKE_C_COMPILER} PATH)
  set(CMAKE_INSTALL_PREFIX "${MINGW_PATH}/..")
endif()
install(FILES "hiredis.h" "async.h" DESTINATION include/${PROJECT_NAME}
        COMPONENT headers)
install(DIRECTORY adapters/ DESTINATION include/${PROJECT_NAME}/adapters
        FILES_MATCHING PATTERN "*.h")
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    COMPONENT libraries)
#set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY TRUE)
#add_dependency(install all_but_tests)

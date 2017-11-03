cmake_minimum_required(VERSION 2.8)

set (TARGET_NAME commontest)
set (TARGET_LIB_NAME libcommon)

project(${TARGET_NAME})

# ---[ cpp 11
set(CMAKE_CXX_FLAGS   "-std=c++11")             # c++11

# ---[ Version
set(COMMON_TEST_VERSION "1.0.0.1")

# ---[ Output position
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---[ Set Inlcude header file path
include_directories(./header ${CMAKE_BINARY_DIR}/thirdparty/gflags/include)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

#message(STATUS "================= ${OpenCV_LIBS}")
#message(STATUS "xxxxxxxx ${OpenCV_INCLUDE_DIRS}")

# ---[ cpp/h file
aux_source_directory(./header HEADERDIRS)
aux_source_directory(./src/ DIRSRCS)
aux_source_directory(./test/ DIRTESTS)

# ---[ h file folder
file(GLOB_RECURSE HEADERDIRS  *.h *.hpp)
source_group("Include" FILES ${HEADERDIRS}) 

# ---[ third party libraries
add_subdirectory(thirdparty/gflags)

# ---[ Add all files to Target project
add_library(${TARGET_LIB_NAME} ${HEADERDIRS} ${DIRSRCS})

add_executable(${TARGET_NAME} ${DIRTESTS})

if(UNIX)
message(STATUS "=== LINUX ===")
target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS} ${TARGET_LIB_NAME} gflags libpthread.so)
else(UNIX)
message(STATUS "=== WINDOWS ===")
target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS} ${TARGET_LIB_NAME} gflags)
endif(UNIX)

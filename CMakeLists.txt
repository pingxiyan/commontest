cmake_minimum_required(VERSION 2.8)

set (TARGET_NAME commontest)

project(${TARGET_NAME})


# ---[ Version
set(COMMON_TEST_VERSION "1.0.0.1")

# ---[ Output position
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---[ Set Inlcude header file path
include_directories(./header)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

message(STATUS "================= ${OpenCV_LIBS}")
message(STATUS "xxxxxxxx ${OpenCV_INCLUDE_DIRS}")

# ---[ cpp/h file
aux_source_directory(./header HEADERDIRS)
aux_source_directory(./src/ DIRSRCS)
aux_source_directory(./test/ DIRSRCS)

# ---[ h file folder
file(GLOB_RECURSE HEADERDIRS  *.h *.hpp)
source_group("Include" FILES ${HEADERDIRS}) 

# ---[ Add all files to Target project
add_executable(${TARGET_NAME} ${HEADERDIRS} ${DIRSRCS})


target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS})

if(UNIX)
target_link_libraries(${TARGET_NAME} ${OpenCV_LIBS} pthread.a)
endif(UNIX)

cmake_minimum_required(VERSION 2.8)

set (TARGET_OBJ commontest)

project(${TARGET_OBJ})


# ---[ Version
set(COMMON_TEST_VERSION "1.0.0.1")

# ---[ Output position
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---[ Set Inlcude header file path
include_directories(./include)

find_package(OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

message(STATUS "================= ${OpenCV_LIBS}")
message(STATUS "xxxxxxxx ${OpenCV_INCLUDE_DIRS}")

# ---[ cpp/h file
aux_source_directory(./include HEADERDIRS)
aux_source_directory(./CommonTest/ DIRSRCS)
aux_source_directory(./test/ DIRSRCS)

# ---[ Add all files to Target project
add_executable(${TARGET_OBJ} ${HEADERDIRS} ${DIRSRCS})


target_link_libraries(${TARGET_OBJ} ${OpenCV_LIBS})

if(UNIX)
target_link_libraries(${TARGET_OBJ} ${OpenCV_LIBS} pthread.a)
endif(UNIX)

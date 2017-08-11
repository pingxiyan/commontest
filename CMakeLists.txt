cmake_minimum_required(VERSION 2.8)

project(CommonTest)

# ---[ Version
set(COMMON_TEST_VERSION "1.0.0.1")

# ---[ Output position
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---[ Set Inlcude header file path
include_directories(./)

# ---[ 3rd OpenCV
# find_package(OpenCV REQUIRED)

# ---[ cpp/h file
aux_source_directory(./ HEADERDIRS)
aux_source_directory(./CommonTest/ DIRSRCS)

# ---[ Add all files to Target project
add_executable(CommonTest ${HEADERDIRS} ${DIRSRCS})

target_link_libraries(CommonTest ${OpenCV_LIBS})
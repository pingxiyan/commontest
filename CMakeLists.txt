cmake_minimum_required(VERSION 2.8)

project(CommonTest)


######################################
if(WIN32)
set(OpenCV_Include "../../../svn_database/trunk/3rdParty/opencv30/header")
set(OpenCV_LibPath "../../../svn_database/trunk/3rdParty/opencv30/lib/x64/vc12/lib")
set(OpenCV_LIBS_D "opencv_world300d.lib")
set(OpenCV_LIBS_R "opencv_world300.lib")
endif(WIN32)

# ---[ Version
set(COMMON_TEST_VERSION "1.0.0.1")

# ---[ Output position
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---[ Set Inlcude header file path
include_directories(./)

# ---[ 3rd OpenCV ===================
if(WIN32)
include_directories(${OpenCV_Include})
LINK_DIRECTORIES(${OpenCV_LibPath})
endif(WIN32)

if(UNIX)
find_package(OpenCV REQUIRED)
endif(UNIX)


# ---[ cpp/h file
aux_source_directory(./ HEADERDIRS)
aux_source_directory(./CommonTest/ DIRSRCS)
aux_source_directory(./test/ DIRSRCS)

# ---[ Add all files to Target project
add_executable(CommonTest ${HEADERDIRS} ${DIRSRCS})

target_link_libraries(CommonTest debug ${OpenCV_LIBS_D})
target_link_libraries(CommonTest optimized ${OpenCV_LIBS_R})

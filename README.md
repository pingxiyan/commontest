# CommonTest
This is test project, include some common test functions.

Dependency: OpenCV

Organized by CMake (> 2.8)

Sandy Yann

Aug. 4 2017

Initial version was writen during I worked at KEDACOM, Thanks KEDACOM for my first six years after graduation.


# Add gflags thirdparty lib, modify CMakeLists.txt as follow:
include_directories(${CMAKE_BINARY_DIR}/thirdparty/gflags/include)
add_subdirectory(thirdparty/gflags)
target_link_libraries(${TARGET_NAME} gflags)

gflags will be used as submodule
Submodule usage:
1: git submodule add https://github.com/gflags/gflags.git ./gflags	// add submodule
2: git submodule init		// init submodule code folder
3: git submodule update		// get submodule code

#
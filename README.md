## CommonTest
This is test project, include some common test functions. <br>
Dependency: OpenCV <br>
Organized by CMake (> 2.8) <br>
Sandy Yann <br>
Aug. 4 2017 <br>
 <br>
Initial version was writen during I worked at KEDACOM, Thanks KEDACOM for my first six years after graduation.
 <br>
## Window cmake use OpenCV
OpenCV_DIR = "yourself git clone opencv code and build position"\build
 <br>
#### For example:
OpenCV_DIR = C:\XipingYan_Code\OpenSourceCode\mygithub\opencv\build
 <br>
## Add gflags thirdparty lib, modify CMakeLists.txt as follow:
include_directories(${CMAKE_BINARY_DIR}/thirdparty/gflags/include)<br>
add_subdirectory(thirdparty/gflags)<br>
target_link_libraries(${TARGET_NAME} gflags)<br>
 <br>
#### Submodule
gflags will be used as submodule<br>
Submodule usage:
1. git submodule add https://github.com/gflags/gflags.git ./gflags <tb> // add submodule
2. git submodule init	 <tb> // init submodule code folder
3. git submodule update  <tb> // get submodule code


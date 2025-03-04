# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 3.10)

# Set the name and the supported language of the project
project(Simple_GTK_OpenCV C CXX)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)

pkg_check_modules(OPENCV REQUIRED IMPORTED_TARGET opencv4)

# add the main code, and link the target_link_libraries
add_executable(my_app main.cpp myWindow.cpp )
target_link_libraries(my_app PUBLIC PkgConfig::GTKMM)
target_link_libraries(my_app PUBLIC PkgConfig::OPENCV)

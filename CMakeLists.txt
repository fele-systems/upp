# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(upp CXX)

# Dependencies from conan
find_package(fmt)
find_package(Crow)

# Main
add_executable("${PROJECT_NAME}"
            src/main.cpp)

target_link_libraries("${PROJECT_NAME}"
            fmt::fmt
            Crow::Crow)
           
target_include_directories("${PROJECT_NAME}" PRIVATE
            include)

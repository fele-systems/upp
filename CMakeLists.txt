# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(upp CXX)


# Dependencies from conan
find_package(fmt)
find_package(Crow)
find_package(yaml-cpp)

# Main
add_executable("${PROJECT_NAME}"
            src/main.cpp src/server.cpp src/config.cpp)

target_compile_features("${PROJECT_NAME}" PRIVATE cxx_std_17)

target_link_libraries("${PROJECT_NAME}"
            fmt::fmt
            Crow::Crow
            yaml-cpp::yaml-cpp)
           
target_include_directories("${PROJECT_NAME}" PRIVATE
            ./include)

add_executable(day09 main.cpp)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

find_package(Threads REQUIRED)

target_link_libraries(day09)
set_project_warnings(day09)


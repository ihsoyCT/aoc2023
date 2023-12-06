add_executable(day06 main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(day06 Threads::Threads)

set_project_warnings(day06)
add_executable(day05 main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(day05 Threads::Threads)

set_project_warnings(day05)
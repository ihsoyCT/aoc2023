add_executable(day07 main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(day07 scn::scn)
set_project_warnings(day07)


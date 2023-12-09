add_executable(day08 main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(day08 scn::scn Boost::unordered)


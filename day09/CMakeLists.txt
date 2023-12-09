add_executable(day09 main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(day09 scn::scn Boost::unordered)

message(FATAL_ERROR ${CMAKE_CXX_FLAGS_BACKUP})
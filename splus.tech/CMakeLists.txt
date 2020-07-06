project (splus)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

set(CMAKE_CXX_COMPILER g++-10)
set(CMAKE_CXX_FLAGS "-std=c++17")
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -pedantic")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unknown-pragmas")
string(APPEND CMAKE_CXX_FLAGS " -Wconversion")
string(APPEND CMAKE_CXX_FLAGS " -O2 -march=native")
#string(APPEND CMAKE_CXX_FLAGS " -g -fno-omit-frame-pointer -O0")
#string(APPEND CMAKE_CXX_FLAGS " -fsanitize=undefined,address")

add_executable(server.exe 
    Source.cpp
    TcpListen.cpp
    WebS.cpp
    Util.cpp
)

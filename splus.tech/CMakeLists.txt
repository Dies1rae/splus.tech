project (splus)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -pedantic")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unknown-pragmas")
string(APPEND CMAKE_CXX_FLAGS " -Wconversion")

add_executable(server.exe 
    Source.cpp
    TcpListen.cpp
    WebS.cpp
)

project (splus)
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_executable(server.exe 
    Source.cpp
    TcpListen.cpp
    WebS.cpp
)

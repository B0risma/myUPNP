cmake_minimum_required(VERSION 3.5)

project(UPNPTest LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-pthread)


#upnp
set(UPNPLIB_SRC /usr/lib/)
set(UPNPLIB_INC /usr/include/miniupnpc)

#srcs
file( GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
file( GLOB INCS ${PROJECT_SOURCE_DIR}/inc/*.h)

add_executable(UPNPTest 
    testUPNP.cpp
    ${SRCS})

target_include_directories(UPNPTest PUBLIC 
"${PROJECT_SOURCE_DIR}"
"${PROJECT_SOURCE_DIR}/inc"
"${UPNPLIB_INC}")
#target_link_libraries(UPNPTest miniupnpc)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(UPNPTest PRIVATE Threads::Threads miniupnpc)


include(GNUInstallDirs)

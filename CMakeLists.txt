cmake_minimum_required(VERSION 3.10)

project(OpenHIL 
    VERSION 0.1.0 
    DESCRIPTION "Open-source Hardware-in-the-Loop simulation framework"
    LANGUAGES CXX
)

# Zahtevamo C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Uključujemo upozorenja prevodioca radi čistog koda
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Opcija za uključivanje build-a unit testova
option(BUILD_TESTS "Build OpenHIL unit tests" ON)

# Dodajemo potprojekte (modul jezgra i testove)
add_subdirectory(core)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
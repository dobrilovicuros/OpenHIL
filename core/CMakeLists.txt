cmake_minimum_required(VERSION 3.10)
# Pravimo biblioteku jezgra od naših .cpp fajlova
add_library(openhil_core
    src/signals/signal_set.cpp
    src/plant/plant.cpp
)

# Definišemo include putanje (tako da fajlovi vide <openhil/signals/signal.hpp>)
target_include_directories(openhil_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Zahtevamo C++20 za ovu biblioteku
target_compile_features(openhil_core PUBLIC cxx_std_20)
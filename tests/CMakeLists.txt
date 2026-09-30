cmake_minimum_required(VERSION 3.10)

# Pravimo izvršni fajl za testiranje signala
add_executable(test_signals
    unit/test_signals.cpp
)

# Povezujemo test sa našom core bibliotekom
target_link_libraries(test_signals
    PRIVATE
        openhil_core
)

# Registrujemo test u CTest sistemu
add_test(NAME test_signals COMMAND test_signals)
cmake_minimum_required(VERSION 3.10)


add_executable(test_signals unit/test_signals.cpp)

add_executable(test_solvers unit/test_solvers.cpp)

add_executable(test_dc_motor unit/test_dc_motor.cpp)

add_executable(test_scheduler unit/test_scheduler.cpp)

add_executable(test_scheduler_stress unit/test_scheduler_stress.cpp)

target_link_libraries(test_scheduler_stress
    PRIVATE
        openhil_core
)

target_link_libraries(test_dc_motor 
    PRIVATE openhil_core)

target_link_libraries(test_signals
    PRIVATE
        openhil_core
)

target_link_libraries(test_solvers
    PRIVATE
        openhil_core
)

target_link_libraries(test_scheduler
    PRIVATE
        openhil_core
)



# Registrujemo test u CTest sistemu
add_test(NAME test_signals COMMAND test_signals)

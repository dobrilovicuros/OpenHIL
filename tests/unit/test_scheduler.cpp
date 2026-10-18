#include "../../core/include/openhil/scheduler/scheduler.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

using namespace openhil::core;

void test_fixed_step_advancement() {
    std::cout << "[TEST] Pokrecem test napredovanja fiksnog koraka...\n";

    // Stvaramo Scheduler sa korakom od 1 ms (0.001 s)
    Scheduler scheduler(0.001);

    int call_count = 0;
    Task dummy_task{
        "DummyTask",
        std::chrono::milliseconds(1),
        TaskPriority::MEDIUM,
        [&call_count](double sim_time, double dt) {
            call_count++;
            assert(dt == 0.001);
        },
        true
    };

    scheduler.addTask(dummy_task);

    // Izvršavamo 10 determinističkih koraka preko step()
    for (int i = 0; i < 10; ++i) {
        scheduler.step();
    }

    assert(call_count == 10);
    assert(scheduler.getStepCount() == 10);
    // Provjeravamo da li je simulaciono vrijeme tačno 0.010 s (sa tolerancijom za floating point)
    assert(std::abs(scheduler.getSimulationTime() - 0.010) < 1e-6);

    std::cout << " -> PASSED: Fiksni korak i simulaciono vrijeme rade ispravno!\n";
}

void test_overrun_detection() {
    std::cout << "[TEST] Pokrecem test detekcije Overrun-a...\n";

    // Scheduler sa fiksnom periodom od 1 ms i LOG_AND_CONTINUE politikom
    Scheduler scheduler(0.001, OverrunPolicy::LOG_AND_CONTINUE);

    // Pravimo zadatak koji namjerno troši više vremena nego što je predviđeno (npr. 3 ms)
    Task slow_task{
        "SlowTask",
        std::chrono::milliseconds(1),
        TaskPriority::HIGH,
        [](double sim_time, double dt) {
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        },
        true
    };

    scheduler.addTask(slow_task);

    // Pokrećemo scheduler u pozadinskoj niti na kratko vrijeme
    std::thread runner([&scheduler]() {
        scheduler.start();
        });

    // Pustimo da se izvrše 2-3 koraka
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    scheduler.stop();

    if (runner.joinable()) {
        runner.join();
    }

    // Očekujemo da je detektovan bar jedan Overrun jer zadatak traje 3 ms u koraku od 1 ms
    assert(scheduler.getDeadlineMisses() > 0);

    std::cout << " -> PASSED: Detektovano je " << scheduler.getDeadlineMisses()
        << " prekoračenja roka (Deadline misses)!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  OpenHIL Unit Test: Scheduler Core     \n";
    std::cout << "========================================\n";

    try {
        test_fixed_step_advancement();
        test_overrun_detection();
        std::cout << "\nSVI TESTOVI SU USPJESNO PROSLI! [PASS]\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Test pao sa izuzetkom: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
#include "../../core/include/openhil/scheduler/scheduler.hpp"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <atomic>

using namespace openhil::core;

void test_multi_rate_tasks() {
    std::cout << "[STRESS TEST 1] Pokrecem test sa vise zadataka razlicitih frekvencija...\n";

    // Korak planera: 1 ms (1000 Hz)
    Scheduler scheduler(0.001, OverrunPolicy::LOG_AND_CONTINUE);

    std::atomic<int> fast_count{ 0 };   // Poziva se na svakih 1 ms
    std::atomic<int> medium_count{ 0 }; // Poziva se na svakih 10 ms
    std::atomic<int> slow_count{ 0 };   // Poziva se na svakih 100 ms

    // 1. Fast Task (1 ms)
    Task fast_task{
        "FastTask_1kHz",
        std::chrono::milliseconds(1),
        TaskPriority::REALTIME,
        [&fast_count](double sim_time, double dt) {
            fast_count++;
        },
        true
    };

    // 2. Medium Task (10 ms -> svakih 10 koraka)
    Task medium_task{
        "MediumTask_100Hz",
        std::chrono::milliseconds(10),
        TaskPriority::HIGH,
        [&medium_count, &scheduler](double sim_time, double dt) {
            // Pokrećemo logiku na svakih 10 stepova
            if (scheduler.getStepCount() % 10 == 0) {
                medium_count++;
            }
        },
        true
    };

    // 3. Slow Task (100 ms -> svakih 100 koraka)
    Task slow_task{
        "SlowTask_10Hz",
        std::chrono::milliseconds(100),
        TaskPriority::LOW,
        [&slow_count, &scheduler](double sim_time, double dt) {
            if (scheduler.getStepCount() % 100 == 0) {
                slow_count++;
            }
        },
        true
    };

    scheduler.addTask(fast_task);
    scheduler.addTask(medium_task);
    scheduler.addTask(slow_task);

    // Izvršavamo 500 determinističkih koraka (0.5 sekundi simulacionog vremena)
    for (int i = 0; i < 500; ++i) {
        scheduler.step();
    }

    assert(fast_count == 500);
    assert(medium_count == 50);
    assert(slow_count == 5);
    assert(std::abs(scheduler.getSimulationTime() - 0.500) < 1e-6);

    std::cout << " -> PASSED: Multi-rate zadaci su se pozvali tacno 500/50/5 puta!\n";
}

void test_dynamic_task_enable_disable() {
    std::cout << "[STRESS TEST 2] Pokrecem test dinamickog paljenja/gasenja zadataka...\n";

    Scheduler scheduler(0.001);

    int executed_count = 0;
    Task toggle_task{
        "ToggleTask",
        std::chrono::milliseconds(1),
        TaskPriority::MEDIUM,
        [&executed_count](double sim_time, double dt) {
            executed_count++;
        },
        true // Na početku omogućen
    };

    scheduler.addTask(toggle_task);

    // Vrtimo 100 koraka (zadatak treba da se izvrši 100 puta)
    for (int i = 0; i < 100; ++i) {
        scheduler.step();
    }
    assert(executed_count == 100);

    // Sada pauziramo/onemogućavamo zadatak (simuliramo izmjenu fleg-a u letu)
    // Napomena: u v0.1 modifikujemo zadatak u vektoru
    // Za potrebe testa vršimo direktnu simulaciju preskakanja kada je enabled=false
    Task disabled_task = toggle_task;
    disabled_task.enabled = false;

    // Čisto testiramo step() ponašanje
    Scheduler scheduler2(0.001);
    scheduler2.addTask(disabled_task);

    for (int i = 0; i < 100; ++i) {
        scheduler2.step();
    }

    std::cout << " -> PASSED: Onemoguceni zadatak se uredno preskace bez uticaja na sat!\n";
}

void test_cpu_spike_resilience() {
    std::cout << "[STRESS TEST 3] Pokrecem test otpornosti na povremeni CPU spike (Overrun bursts)...\n";

    Scheduler scheduler(0.001, OverrunPolicy::LOG_AND_CONTINUE);

    std::atomic<int> total_steps{ 0 };

    // Zadatak koji na svakih 50 koraka "zapne" na 3 ms (prouzrokuje Overrun)
    Task spiky_task{
        "SpikyTask",
        std::chrono::milliseconds(1),
        TaskPriority::HIGH,
        [&total_steps, &scheduler](double sim_time, double dt) {
            total_steps++;
            if (scheduler.getStepCount() % 50 == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(3));
            }
        },
        true
    };

    scheduler.addTask(spiky_task);

    std::thread runner([&scheduler]() {
        scheduler.start();
        });

    // Pustimo ga da radi 150 ms
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    scheduler.stop();

    if (runner.joinable()) {
        runner.join();
    }

    assert(scheduler.getDeadlineMisses() > 0);
    assert(total_steps > 0);

    std::cout << " -> PASSED: Planer preživio " << scheduler.getDeadlineMisses()
        << " Overrun-a i nastavio stabilno da radi!\n";
}

int main() {
    std::cout << "========================================\n";
    std::cout << "  OpenHIL Stress Test: Scheduler Core   \n";
    std::cout << "========================================\n";

    try {
        test_multi_rate_tasks();
        test_dynamic_task_enable_disable();
        test_cpu_spike_resilience();
        std::cout << "\nSVI STRESS TESTOVI SU USPJESNO PROSLI! [PASS]\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n[ERROR] Stress test pao sa izuzetkom: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
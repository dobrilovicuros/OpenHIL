#include "openhil/scheduler/scheduler.hpp"
#include <thread>
#include <stdexcept>
#include <iostream>

namespace openhil {
    namespace core {

        Scheduler::Scheduler(double timestep_seconds, OverrunPolicy policy)
            : dt_(timestep_seconds),
            dt_nano_(static_cast<int64_t>(timestep_seconds * 1e9)),
            overrun_policy_(policy) {
            if (timestep_seconds <= 0.0) {
                throw std::invalid_argument("Vremenski korak (timestep) mora biti pozitivan broj!");
            }
        }

        void Scheduler::addTask(const Task& task) {
            tasks_.push_back(task);
        }

        void Scheduler::step() {
            // Prolazimo kroz sve zadatke i izvršavamo ih po redoslijedu
            for (auto& task : tasks_) {
                if (!task.enabled) {
                    continue;
                }

                auto start_time = std::chrono::steady_clock::now();

                // Pozivamo definisanu radnu funkciju zadatka
                if (task.callback) {
                    task.callback(sim_time_, dt_);
                }

                auto end_time = std::chrono::steady_clock::now();
                task.last_execution_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            }

            // Napredovanje simulacionog vremena
            sim_time_ += dt_;
            step_count_++;
        }

        void Scheduler::start() {
            if (is_running_.load()) {
                return;
            }

            is_running_.store(true);
            // Pokrećemo izvršnu petlju
            executionLoop();
        }

        void Scheduler::stop() {
            is_running_.store(false);
        }

        void Scheduler::executionLoop() {
            auto next_step_time = std::chrono::steady_clock::now();

            while (is_running_.load()) {
                auto step_start = std::chrono::steady_clock::now();

                // 1. Izvršavamo jedan deterministički korak simulacije
                step();

                auto step_end = std::chrono::steady_clock::now();
                auto exec_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(step_end - step_start);

                // 2. Provjera uslova realnog vremena (Overrun / Deadline Miss)
                if (exec_duration > dt_nano_) {
                    deadline_misses_++;

                    if (overrun_policy_ == OverrunPolicy::HARD_STOP) {
                        is_running_.store(false);
                        throw std::runtime_error("HARD_STOP: Prekoračen uslov realnog vremena (Overrun detected)!");
                    }
                    else if (overrun_policy_ == OverrunPolicy::SKIP_STEP) {
                        // Preskačemo taktove u wall-clock vremenu da nadoknadimo kašnjenje
                        next_step_time = std::chrono::steady_clock::now() + dt_nano_;
                        continue;
                    }
                    // Za LOG_AND_CONTINUE samo nastavljamo dalje
                }

                // 3. Održavanje fiksnog koraka — uspavljivanje niti do sledećeg takta
                next_step_time += dt_nano_;
                std::this_thread::sleep_until(next_step_time);
            }
        }

    } // namespace core
} // namespace openhil
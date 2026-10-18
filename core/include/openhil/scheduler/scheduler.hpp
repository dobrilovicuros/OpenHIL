#ifndef OPENHIL_CORE_SCHEDULER_HPP
#define OPENHIL_CORE_SCHEDULER_HPP

#include "Task.hpp"
#include <vector>
#include <atomic>
#include <chrono>

namespace openhil {
    namespace core {

        enum class OverrunPolicy {
            HARD_STOP,       // Odmah zaustavi simulaciju pri prvom prekoračenju
            SKIP_STEP,       // Preskoči trenutni korak da uhvati priključak
            LOG_AND_CONTINUE // Zabilježi prekoračenje i nastavi dalje
        };

        class Scheduler {
        public:
            explicit Scheduler(double timestep_seconds, OverrunPolicy policy = OverrunPolicy::LOG_AND_CONTINUE);
            ~Scheduler() = default;

            // Registracija radnih zadataka
            void addTask(const Task& task);

            // Kontrola izvršavanja simulacije
            void start();
            void stop();
            void step(); // Izvršava tačno jedan deterministički korak (korisno za unit testove)

            // Dohvatanje stanja simulacionog sata i statistike
            [[nodiscard]] double getSimulationTime() const { return sim_time_; }
            [[nodiscard]] uint64_t getStepCount() const { return step_count_; }
            [[nodiscard]] uint64_t getDeadlineMisses() const { return deadline_misses_; }
            [[nodiscard]] bool isRunning() const { return is_running_.load(); }

        private:
            void executionLoop();

            double dt_;                                // Korak simulacije u sekundama (npr. 0.001 s)
            std::chrono::nanoseconds dt_nano_;         // Fiksni korak u nanosekundama
            double sim_time_{ 0.0 };                     // Simulaciono vrijeme (t_sim)
            uint64_t step_count_{ 0 };                   // Ukupan broj izvršenih koraka

            std::atomic<bool> is_running_{ false };      // Fleg za rad niti
            OverrunPolicy overrun_policy_;
            uint64_t deadline_misses_{ 0 };             // Brojač detektovanih Overrun-a

            std::vector<Task> tasks_;                  // Lista registrovanih zadataka
        };

    } // namespace core
} // namespace openhil

#endif // OPENHIL_CORE_SCHEDULER_HPP
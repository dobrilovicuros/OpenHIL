#ifndef OPENHIL_CORE_TASK_HPP
#define OPENHIL_CORE_TASK_HPP

#include <string>
#include <functional>
#include <chrono>

namespace openhil {
    namespace core {

        // Callback funkcija prima trenutno simulaciono vrijeme i dt (vremenski korak)
        using TaskCallback = std::function<void(double sim_time, double dt)>;

        enum class TaskPriority {
            LOW = 0,
            MEDIUM,
            HIGH,
            REALTIME
        };

        struct Task {
            std::string name;
            std::chrono::nanoseconds period{ 0 };
            TaskPriority priority{ TaskPriority::MEDIUM };
            TaskCallback callback;
            bool enabled{ true };

            // Statistika i dijagnostika izvršavanja
            std::chrono::nanoseconds last_execution_time{ 0 };
            uint64_t overrun_count{ 0 };
        };

    } // namespace core
} // namespace openhil

#endif // OPENHIL_CORE_TASK_HPP
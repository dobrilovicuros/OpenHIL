#ifndef OPENHIL_SIGNALS_SIGNAL_HPP
#define OPENHIL_SIGNALS_SIGNAL_HPP

#include <string>
#include <cstdint>
#include <utility>

namespace openhil::signals {

    enum class SignalDirection {
        INPUT,
        OUTPUT,
        INTERNAL
    };

    enum class SignalType {
        DOUBLE,
        INT,
        BOOL
    };

    struct Signal {
        std::string name;
        double value{ 0.0 };
        SignalType type{ SignalType::DOUBLE };
        std::string unit{ "" };
        SignalDirection direction{ SignalDirection::INTERNAL };
        double timestamp{ 0.0 };

        // Pomoćni konstruktor za lakše kreiranje
        Signal() = default;
        Signal(std::string name, double value, std::string unit = "",
            SignalDirection direction = SignalDirection::INTERNAL, double timestamp = 0.0)
            : name(std::move(name)), value(value), type(SignalType::DOUBLE),
            unit(std::move(unit)), direction(direction), timestamp(timestamp) {
        }
    };

} // namespace openhil::signals

#endif // OPENHIL_SIGNALS_SIGNAL_HPP
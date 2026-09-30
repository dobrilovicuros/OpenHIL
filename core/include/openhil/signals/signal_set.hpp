#ifndef OPENHIL_SIGNALS_SIGNAL_SET_HPP
#define OPENHIL_SIGNALS_SIGNAL_SET_HPP

#include "openhil/signals/signal.hpp"
#include <unordered_map>
#include <string>
#include <optional>
#include <vector>

namespace openhil::signals {

    class SignalSet {
    public:
        SignalSet() = default;

        // Dodaje novi signal ili prepisuje postojeći
        void add(const Signal& signal) {
            signals_[signal.name] = signal;
        }

        // Proverava da li signal postoji
        bool has(const std::string& name) const {
            return signals_.find(name) != signals_.end();
        }

        // Dobija pokazivač na signal ako postoji (omogućava izmenu vrednosti)
        Signal* get(const std::string& name) {
            auto it = signals_.find(name);
            if (it != signals_.end()) {
                return &(it->second);
            }
            return nullptr;
        }

        // Constant verzija getera
        const Signal* get(const std::string& name) const {
            auto it = signals_.find(name);
            if (it != signals_.end()) {
                return &(it->second);
            }
            return nullptr;
        }

        // Brzo postavljanje vrednosti
        bool set_value(const std::string& name, double value, double timestamp = 0.0) {
            auto* sig = get(name);
            if (sig) {
                sig->value = value;
                sig->timestamp = timestamp;
                return true;
            }
            return false;
        }

        // Vraća sve signale u mapi
        const std::unordered_map<std::string, Signal>& all() const {
            return signals_;
        }

    private:
        std::unordered_map<std::string, Signal> signals_;
    };

} // namespace openhil::signals

#endif // OPENHIL_SIGNALS_SIGNAL_SET_HPP
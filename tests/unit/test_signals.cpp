#include "openhil/signals/signal.hpp"
#include "openhil/signals/signal_set.hpp"
#include <iostream>
#include <cassert>

using namespace openhil::signals;

int main() {
    std::cout << "--- Pokretanje OpenHIL Signal Unit Testa ---" << std::endl;

    SignalSet set;

    // 1. Dodavanje signala
    Signal speed_sig("motor.speed", 1200.0, "rpm", SignalDirection::OUTPUT, 0.0);
    Signal voltage_sig("motor.voltage", 12.0, "V", SignalDirection::INPUT, 0.0);

    set.add(speed_sig);
    set.add(voltage_sig);

    // 2. Provera postojanja
    assert(set.has("motor.speed") == true);
    assert(set.has("motor.voltage") == true);
    assert(set.has("motor.current") == false);

    // 3. Provera citanja vrednosti
    const auto* sig = set.get("motor.speed");
    assert(sig != nullptr);
    assert(sig->value == 1200.0);
    assert(sig->unit == "rpm");

    // 4. Provera izmene vrednosti
    bool updated = set.set_value("motor.speed", 1500.0, 0.001);
    assert(updated == true);
    assert(set.get("motor.speed")->value == 1500.0);
    assert(set.get("motor.speed")->timestamp == 0.001);

    std::cout << "[SUCCESS] Svi testovi za Signal i SignalSet su USPESNO prošli!" << std::endl;
    return 0;
}
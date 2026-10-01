#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>

#include "openhil/solvers/euler_solver.hpp"
#include "openhil/solvers/rk4_solver.hpp"
#include "openhil/plant/plant.hpp"

// Pomagalo: Izmještena (Dummy) Plant klasa za testiranje sistema dx/dt = -x
class ExponentialDecayPlant : public openhil::plant::Plant {
private:
    std::vector<double> current_state_ = { 1.0 }; // x(0) = 1.0

public:
    void initialize() override {}

    const std::vector<double>& state() const override {
        return current_state_;
    }

    void set_state(const std::vector<double>& state) override {
        current_state_ = state;
    }

    std::vector<double> compute_derivatives(
        double t,
        const std::vector<double>& state,
        const openhil::signals::SignalSet& inputs) override {
        // dx/dt = -x
        return { -state[0] };
    }

    void update_outputs(double t) override {}
    openhil::signals::SignalSet inputs() const override { return {}; }
    openhil::signals::SignalSet outputs() const override { return {}; }
};

int main() {
    std::cout << "========================================\n";
    std::cout << " RUNNING OPENHIL SOLVERS UNIT TEST\n";
    std::cout << "========================================\n\n";

    double dt = 0.1;       // Vremenski korak 100 ms
    double duration = 1.0; // Simuliramo do t = 1.0 s
    int steps = static_cast<int>(duration / dt);
    double exact_solution = std::exp(-1.0); // e^(-1) ~ 0.367879

    // ----------------------------------------------------
    // TEST 1: Forward Euler Solver
    // ----------------------------------------------------
    {
        ExponentialDecayPlant plant_euler;
        openhil::solvers::EulerSolver euler_solver;
        openhil::signals::SignalSet dummy_inputs;

        double t = 0.0;
        for (int i = 0; i < steps; ++i) {
            euler_solver.step(plant_euler, t, dt, dummy_inputs);
            t += dt;
        }

        double euler_res = plant_euler.state()[0];
        double euler_err = std::abs(euler_res - exact_solution);

        std::cout << "[EULER] Result at t=1.0s: " << euler_res
            << " (Error: " << euler_err << ")\n";

        // Euler sa dt=0.1 ima pogrešku reda ~0.02
        assert(euler_err < 0.05 && "Euler solver Error unexpectedly high!");
        std::cout << "[EULER] PASSED!\n\n";
    }

    // ----------------------------------------------------
    // TEST 2: RK4 Solver
    // ----------------------------------------------------
    {
        ExponentialDecayPlant plant_rk4;
        openhil::solvers::RK4Solver rk4_solver;
        openhil::signals::SignalSet dummy_inputs;

        double t = 0.0;
        for (int i = 0; i < steps; ++i) {
            rk4_solver.step(plant_rk4, t, dt, dummy_inputs);
            t += dt;
        }

        double rk4_res = plant_rk4.state()[0];
        double rk4_err = std::abs(rk4_res - exact_solution);

        std::cout << "[RK4]   Result at t=1.0s: " << rk4_res
            << " (Error: " << rk4_err << ")\n";

        // RK4 sa dt=0.1 treba imati visoku preciznost (pogreška < 1e-4)
        assert(rk4_err < 1e-4 && "RK4 solver Error unexpectedly high!");
        std::cout << "[RK4]   PASSED!\n\n";
    }

    std::cout << "========================================\n";
    std::cout << " ALL SOLVER TESTS PASSED SUCCESSFULLY!\n";
    std::cout << "========================================\n";

    return 0;
}
#include "openhil/solvers/rk4_solver.hpp"
#include "openhil/plant/plant.hpp"
#include <vector>

namespace openhil::solvers {

    RK4Solver::RK4Solver() = default;

    std::vector<double> RK4Solver::step(
        plant::Plant& plant,
        double t,
        double dt,
        const signals::SignalSet& inputs) {

        // 1. Pročitaj početno stanje x_k
        std::vector<double> xk = plant.state();
        size_t size = xk.size();

        std::vector<double> xk_temp(size);
        std::vector<double> next_state(size);

        // 2. Proračun k1 = f(t, x_k)
        std::vector<double> k1 = plant.compute_derivatives(t, xk, inputs);

        // 3. Proračun k2 = f(t + 0.5*dt, x_k + 0.5*dt*k1)
        for (size_t i = 0; i < size; ++i) {
            xk_temp[i] = xk[i] + 0.5 * dt * k1[i]; // Bilo je +=, mora biti =
        }
        std::vector<double> k2 = plant.compute_derivatives(t + 0.5 * dt, xk_temp, inputs);

        // 4. Proračun k3 = f(t + 0.5*dt, x_k + 0.5*dt*k2)
        for (size_t i = 0; i < size; ++i) {
            xk_temp[i] = xk[i] + 0.5 * dt * k2[i]; // Bilo je +=, mora biti =
        }
        std::vector<double> k3 = plant.compute_derivatives(t + 0.5 * dt, xk_temp, inputs);

        // 5. Proračun k4 = f(t + dt, x_k + dt*k3)
        for (size_t i = 0; i < size; ++i) {
            xk_temp[i] = xk[i] + dt * k3[i]; // Bilo je +=, mora biti =
        }
        std::vector<double> k4 = plant.compute_derivatives(t + dt, xk_temp, inputs);

        // 6. Konačno spajanje: x_{k+1} = x_k + (dt/6) * (k1 + 2*k2 + 2*k3 + k4)
        for (size_t i = 0; i < size; ++i) {
            next_state[i] = xk[i] + (dt / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
        }

        // 7. Zapis u model
        plant.set_state(next_state);
        plant.update_outputs(t + dt);

        return next_state;
    }

} // namespace openhil::solvers
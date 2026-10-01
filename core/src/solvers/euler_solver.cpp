#include "openhil/solvers/euler_solver.hpp"
#include "openhil/plant/plant.hpp"
#include <vector>

namespace openhil::solvers {
	EulerSolver::EulerSolver() = default;

	std::vector<double> EulerSolver::step(
		plant::Plant& plant,
		double t,
		double dt,
		const signals::SignalSet& inputs) {
		// Implementation for Euler solver step
		std::vector<double>xk = plant.state();

		std::vector<double> derivatives = plant.compute_derivatives(t, xk, inputs);

		std::vector<double> next_state;
		next_state.resize(xk.size());

		for (size_t i = 0; i < xk.size(); ++i) {
			next_state[i] = xk[i] + dt * derivatives[i];
		}

		plant.set_state(next_state);
		plant.update_outputs(t + dt);


		return next_state;
	}
} // namespace openhil::solvers
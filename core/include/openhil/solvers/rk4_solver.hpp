#ifndef OPENHIL_SOLVERS_RK4_SOLVER_HPP
#define OPENHIL_SOLVERS_RK4_SOLVER_HPP

#include "openhil/solvers/solver.hpp"
#include <vector>

namespace openhil::solvers {

class RK4Solver : public Solver {
public:
	RK4Solver();
	std::vector<double> step(
		plant::Plant& plant,
		double t,
		double dt,
		const signals::SignalSet& inputs) override;
};

} // namespace openhil::solvers

#endif // OPENHIL_SOLVERS_RK4_SOLVER_HPP

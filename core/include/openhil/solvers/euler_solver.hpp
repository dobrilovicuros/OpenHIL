#ifndef OPENHIL_SOLVERS_EULER_SOLVER_HPP
#define OPENHIL_SOLVERS_EULER_SOLVER_HPP

#include "openhil/solvers/solver.hpp"
#include <vector>

namespace openhil::solvers {

class EulerSolver : public Solver {
public:
	EulerSolver();
	std::vector<double> step(
		plant::Plant& plant,
		double t,
		double dt,
		const signals::SignalSet& inputs) override;
};

} // namespace openhil::solvers

#endif // OPENHIL_SOLVERS_EULER_SOLVER_HPP

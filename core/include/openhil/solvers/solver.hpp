#ifndef OPENHIL_SOLVERS_SOLVER_HPP
#define OPENHIL_SOLVERS_SOLVER_HPP

#include <vector>
#include "openhil/plant/plant.hpp"
#include "openhil/signals/signal_set.hpp"

namespace openhil::solvers {

class Solver {
public:
	virtual ~Solver() = default;
	virtual std::vector<double> step(
		plant::Plant& plant,
		double t,
		double dt,
		const signals::SignalSet& inputs) = 0;
};

} // namespace openhil::solvers

#endif // OPENHIL_SOLVERS_SOLVER_HPP

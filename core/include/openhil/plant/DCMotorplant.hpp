#ifndef OPENHIL_PLANT_DCMOTORPLANT_HPP
#define OPENHIL_PLANT_DCMOTORPLANT_HPP

#include "openhil/plant/plant.hpp"
#include <vector>

namespace openhil::plant {

	struct DCMotorParams {
		double R = 1.0;
		double L = 0.5;
		double Kt = 0.01;
		double Ke = 0.01;
		double J = 0.01;
		double b = 0.1;
	};

	class DCMotorPlant : public Plant {
	public:
		DCMotorPlant();
			explicit DCMotorPlant(const DCMotorParams& params);

			void initialize() override;

			const std::vector<double>& state() const override;
			void set_state(const std::vector<double>& x) override;

		std::vector<double> compute_derivatives(
			double t,
			const std::vector<double>& x,
			const signals::SignalSet& inputs) override;

		void update_outputs(double t) override;

		signals::SignalSet inputs() const override;
		signals::SignalSet outputs() const override;

		double current() const { return state_[0]; }
		double speed() const { return state_[1]; }

	private:

		DCMotorParams params_;

		std::vector<double> state_ = { 0.0, 0.0 }; // [current, speed]
	};
}// namespace openhil::plant

#endif // OPENHIL_PLANT_DCMOTORPLANT_HPP
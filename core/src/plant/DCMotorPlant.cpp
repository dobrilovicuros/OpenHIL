#include "openhil/plant/DCMotorPlant.hpp"

namespace openhil::plant {

    DCMotorPlant::DCMotorPlant() : params_(DCMotorParams{}) {}

    DCMotorPlant::DCMotorPlant(const DCMotorParams& params) : params_(params) {}

    void DCMotorPlant::initialize() {
        state_ = { 0.0, 0.0 }; // Početna struja = 0, brzina = 0
    }

    const std::vector<double>& DCMotorPlant::state() const {
        return state_;
    }

    void DCMotorPlant::set_state(const std::vector<double>& state) {
        if (state.size() == 2) {
            state_ = state;
        }
    }

    std::vector<double> DCMotorPlant::compute_derivatives(
        double t,
        const std::vector<double>& state,
        const signals::SignalSet& inputs) {

        // Ako u signalima ulaza postoje "voltage" i "load_torque", pročitaj ih,
        // u suprotnom podrazumevaj 0.0
        const auto* sigV = inputs.get("voltage");
        double V = sigV ? sigV->value : 0.0;
        const auto* sigT = inputs.get("load_torque");
        double T_L = sigT ? sigT->value : 0.0;

        double i = state[0];
        double omega = state[1];

        // di/dt = (V - R*i - Ke*omega) / L
        double di_dt = (V - params_.R * i - params_.Ke * omega) / params_.L;

        // d_omega/dt = (Kt*i - b*omega - T_L) / J
        double domega_dt = (params_.Kt * i - params_.b * omega - T_L) / params_.J;

        return { di_dt, domega_dt };
    }

    void DCMotorPlant::update_outputs(double t) {
        // Ovde možemo osvežavati izlazne signale ako zatreba
    }

    signals::SignalSet DCMotorPlant::inputs() const {
        return { {"voltage", 0.0}, {"load_torque", 0.0} };
    }

    signals::SignalSet DCMotorPlant::outputs() const {
        return { {"current", state_[0]}, {"speed", state_[1]} };
    }

} // namespace openhil::plant
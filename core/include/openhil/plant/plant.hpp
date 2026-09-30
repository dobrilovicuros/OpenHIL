#ifndef OPENHIL_PLANT_PLANT_HPP
#define OPENHIL_PLANT_PLANT_HPP

#include "openhil/signals/signal_set.hpp"
#include <vector>

namespace openhil::plant {

    class Plant {
    public:
        virtual ~Plant() = default;

        // Inicijalizacija stanja i parametara
        virtual void initialize() = 0;

        // Računanje izvoda stanja \dot{x} = f(t, x, u) za numerički integrator
        virtual std::vector<double> compute_derivatives(
            double t,
            const std::vector<double>& x,
            const signals::SignalSet& inputs) = 0;

        // Pristup unutrašnjem vektoru stanja
        virtual const std::vector<double>& state() const = 0;
        virtual void set_state(const std::vector<double>& x) = 0;

        // Dohvatanje ulaza i izlaza u obliku SignalSet-a
        virtual signals::SignalSet inputs() const = 0;
        virtual signals::SignalSet outputs() const = 0;

        // Ažuriranje izlaznih signala na osnovu trenutnog stanja y = g(t, x, u)
        virtual void update_outputs(double t) = 0;
    };

} // namespace openhil::plant

#endif // OPENHIL_PLANT_PLANT_HPP
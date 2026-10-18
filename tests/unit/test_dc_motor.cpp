#include <iostream>
#include <cmath>
#include <cassert>

#include "openhil/plant/DCMotorPlant.hpp"
#include "openhil/solvers/rk4_solver.hpp"

int main() {
    std::cout << "========================================\n";
    std::cout << " RUNNING DC MOTOR PLANT UNIT TEST\n";
    std::cout << "========================================\n\n";

    openhil::plant::DCMotorParams params;
    params.R = 2.0;    // 2 Ohma
    params.L = 0.5;    // 0.5 H
    params.Kt = 0.1;   // 0.1 Nm/A
    params.Ke = 0.1;   // 0.1 V/(rad/s)
    params.J = 0.01;   // 0.01 kg*m^2
    params.b = 0.02;   // 0.02 Nms/rad

    openhil::plant::DCMotorPlant motor(params);
    openhil::solvers::RK4Solver solver;

    double dt = 0.001; // Step od 1 ms
    double t = 0.0;

    // Nanesemo ulazni napon od 12V
    openhil::signals::SignalSet inputs = {
        {"voltage", 12.0},
        {"load_torque", 0.0}
    };

    std::cout << "Simulating DC Motor step response for 2.0 seconds...\n";
    for (int i = 0; i < 2000; ++i) {
        solver.step(motor, t, dt, inputs);
        t += dt;
    }

    std::cout << "[RESULT] Final Current i(2.0s): " << motor.current() << " A\n";
    std::cout << "[RESULT] Final Speed w(2.0s):   " << motor.speed() << " rad/s\n";

    // Provera fizikalne logike: Brzina i struja moraju biti pozitivne i stabilizovane
    assert(motor.speed() > 0.0 && "Speed should be positive!");
    assert(motor.current() > 0.0 && "Current should be positive!");

    std::cout << "\n========================================\n";
    std::cout << " DC MOTOR PLANT TEST PASSED!\n";
    std::cout << "========================================\n";

    return 0;
}
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "vehicles/auv/api/AuvRpcLibClient.hpp"
#include "common/common_utils/FileSystem.hpp"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <string>
#include <thread>

int main(int argc, char* argv[])
{
    using namespace msr::airlib;

    // Override host with argv[1] or AIRSIM_HOST env var (useful from WSL: pass the Windows host IP).
    std::string host = "localhost";
    if (argc > 1) host = argv[1];
    else if (const char* env = std::getenv("AIRSIM_HOST")) host = env;

    std::cout << "Make sure settings.json has \"SimMode\"=\"Auv\" at root. Press Enter to continue." << std::endl;
    std::cout << "Connecting to " << host << ":" << RpcLibPort << std::endl;
    std::cin.get();

    msr::airlib::AuvRpcLibClient client(host);

    try {
        client.confirmConnection();
        client.enableApiControl(true);
        std::cout << "API Control enabled: " << client.isApiControlEnabled() << std::endl;

        auto state = client.getAuvState();
        std::cout << "state.position    \t" << state.getPosition().transpose() << std::endl;
        std::cout << "state.orientation \t" << state.getOrientation().coeffs().transpose() << std::endl;
        std::cout << "state.timestamp   \t" << state.timestamp << std::endl;

        auto imu_data = client.getImuData();
        std::cout << "imu.angular_velocity    \t" << imu_data.angular_velocity << std::endl;
        std::cout << "imu.linear_acceleration \t" << imu_data.linear_acceleration << std::endl;

        std::cout << "Press Enter to apply forward thrust (50 N) for 3 seconds" << std::endl;
        std::cin.get();
        client.setAuvControls(50.f, 0.f, 0.f, 0.f, 0.f, 0.f);
        std::cout << "Go Forward" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // remove thrust (vehicle decelerates by hydrodynamic drag)
        client.setAuvControls(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
        std::cout << "Thrust off" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::cout << "Press Enter to yaw right (2 N*m) for 3 seconds" << std::endl;
        std::cin.get();
        // positive yaw torque = clockwise (NED)
        client.setAuvControls(0.f, 0.f, 0.f, 0.f, 0.f, 2.f);
        std::cout << "Yaw Right" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        client.setAuvControls(0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
        std::cout << "Torque off" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::cout << "Press Enter to disable API control" << std::endl;
        std::cin.get();
        client.enableApiControl(false);
    }
    catch (rpc::rpc_error& e) {
        const auto msg = e.get_error().as<std::string>();
        std::cout << "Exception raised by the API, something went wrong." << std::endl
                  << msg << std::endl;
    }

    return 0;
}

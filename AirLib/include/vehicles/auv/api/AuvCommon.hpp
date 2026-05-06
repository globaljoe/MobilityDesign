#ifndef water_AuvCommon_hpp
#define water_AuvCommon_hpp

#include "common/Common.hpp"
#include "common/CommonStructs.hpp"
#include "physics/Kinematics.hpp"

namespace msr
{
namespace airlib
{
    // // Structs for thruster state API
    // struct ThrusterParameters
    // {
    //     real_T thrust = 0;
    //     real_T torque_scaler = 0;
    //     real_T speed = 0;

    //     ThrusterParameters()
    //     {
    //     }

    //     ThrusterParameters(const real_T& thrust_val, const real_T& torque_scaler_val, const real_T& speed_val)
    //         : thrust(thrust_val), torque_scaler(torque_scaler_val), speed(speed_val)
    //     {
    //     }

    //     void update(const real_T& thrust_val, const real_T& torque_scaler_val, const real_T& speed_val)
    //     {
    //         thrust = thrust_val;
    //         torque_scaler = torque_scaler_val;
    //         speed = speed_val;
    //     }
    // };

    // struct ThrusterStates
    // {
    //     std::vector<ThrusterParameters> thrusters;
    //     uint64_t timestamp;

    //     ThrusterStates()
    //     {
    //     }
    //     ThrusterStates(const std::vector<ThrusterParameters>& thrusters_val, uint64_t timestamp_val)
    //         : thrusters(thrusters_val), timestamp(timestamp_val)
    //     {
    //     }
    // };

}
} //namespace
#endif
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AsvRpcLibAdaptors_hpp
#define air_AsvRpcLibAdaptors_hpp

#include "common/Common.hpp"
#include "common/CommonStructs.hpp"
#include "api/RpcLibAdaptorsBase.hpp"
#include "common/ImageCaptureBase.hpp"
#include "vehicles/asv/api/AsvApiBase.hpp"

#include "common/common_utils/WindowsApisCommonPre.hpp"
#include "rpc/msgpack.hpp"
#include "common/common_utils/WindowsApisCommonPost.hpp"

namespace msr
{
namespace airlib_rpclib
{

    class AsvRpcLibAdaptors : public RpcLibAdaptorsBase
    {
    public:
        struct AsvControls
        {
            float left_force  = 0.f;  // [N]
            float right_force = 0.f;  // [N]

            MSGPACK_DEFINE_ARRAY(left_force, right_force);

            AsvControls() {}

            AsvControls(const msr::airlib::AsvApiBase::AsvControls& s)
            {
                left_force  = s.left_force;
                right_force = s.right_force;
            }
            msr::airlib::AsvApiBase::AsvControls to() const
            {
                return msr::airlib::AsvApiBase::AsvControls(left_force, right_force);
            }
        };

        struct ThrusterParameters
        {
            msr::airlib::real_T thrust;
            msr::airlib::real_T torque_scaler;
            msr::airlib::real_T speed;

            MSGPACK_DEFINE_ARRAY(thrust, torque_scaler, speed);

            ThrusterParameters()
            {
            }

            ThrusterParameters(const msr::airlib::ThrusterParameters& s)
            {
                thrust = s.thrust;
                torque_scaler = s.torque_scaler;
                speed = s.speed;
            }

            msr::airlib::ThrusterParameters to() const
            {
                return msr::airlib::ThrusterParameters(thrust, torque_scaler, speed);
            }
        };

        struct ThrusterStates
        {
            std::vector<ThrusterParameters> thrusters;
            uint64_t timestamp;

            MSGPACK_DEFINE_ARRAY(thrusters, timestamp);

            ThrusterStates()
            {
            }

            ThrusterStates(const msr::airlib::ThrusterStates& s)
            {
                for (const auto& r : s.thrusters) {
                    thrusters.push_back(ThrusterParameters(r));
                }
                timestamp = s.timestamp;
            }

            msr::airlib::ThrusterStates to() const
            {
                std::vector<msr::airlib::ThrusterParameters> d;
                for (const auto& r : thrusters) {
                    d.push_back(r.to());
                }
                return msr::airlib::ThrusterStates(d, timestamp);
            }
        };

        struct AsvState
        {
            KinematicsState kinematics_estimated;
            uint64_t timestamp;

            MSGPACK_DEFINE_ARRAY(kinematics_estimated, timestamp);

            AsvState()
            {
            }

            AsvState(const msr::airlib::AsvApiBase::AsvState& s)
            {
                timestamp = s.timestamp;
                kinematics_estimated = s.kinematics_estimated;
            }
            msr::airlib::AsvApiBase::AsvState to() const
            {
                return msr::airlib::AsvApiBase::AsvState(
                    kinematics_estimated.to(), timestamp);
            }
        };
    };
}
} //namespace

#endif

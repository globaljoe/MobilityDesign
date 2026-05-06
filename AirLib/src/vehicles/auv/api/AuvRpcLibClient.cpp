// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//in header only mode, control library is not available
#ifndef AIRLIB_HEADER_ONLY
//RPC code requires C++14. If build system like Unreal doesn't support it then use compiled binaries
#ifndef AIRLIB_NO_RPC
//if using Unreal Build system then include precompiled header file first

#include "vehicles/auv/api/AuvRpcLibClient.hpp"

#include "common/Common.hpp"
#include "common/ClockFactory.hpp"
#include <thread>
STRICT_MODE_OFF

#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK

#ifdef nil
#undef nil
#endif // nil

#include "common/common_utils/WindowsApisCommonPre.hpp"
#undef FLOAT
#undef check
#include "rpc/client.h"
//TODO: HACK: UE4 defines macro with stupid names like "check" that conflicts with msgpack library
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
#include "common/common_utils/WindowsApisCommonPost.hpp"

#include "vehicles/auv/api/AuvRpcLibAdaptors.hpp"

STRICT_MODE_ON
#ifdef _MSC_VER
__pragma(warning(disable : 4239))
#endif

    namespace msr
{
    namespace airlib
    {

        typedef msr::airlib_rpclib::AuvRpcLibAdaptors AuvRpcLibAdaptors;

        AuvRpcLibClient::AuvRpcLibClient(const string& ip_address, uint16_t port, float timeout_sec)
            : RpcLibClientBase(ip_address, port, timeout_sec)
        {
        }

        AuvRpcLibClient::~AuvRpcLibClient()
        {
        }

        // AuvRpcLibClient* AuvRpcLibClient::moveByVelocityAsync(float vx, float vy, float vz, float yaw, float duration,
        //                                                       const std::string& vehicle_name)
        // {
        //     return static_cast<rpc::client*>(getClient())->async_call("moveByVelocity", vx, vy, vz, yaw, duration, vehicle_name).to();
        // }

        // void AuvRpcLibClient::setAuvControls(const AuvApiBase::AuvControls& controls, const std::string& vehicle_name)
        void AuvRpcLibClient::setAuvControls(float fx, float fy, float fz, float roll, float pitch, float yaw, const std::string& vehicle_name)
        {
            AuvApiBase::AuvControls controls(
                Vector3r(fx, fy, fz),
                Vector3r(roll, pitch, yaw)
            );
            static_cast<rpc::client*>(getClient())->call("setAuvControls", AuvRpcLibAdaptors::AuvControls(controls), vehicle_name);
        }

        AuvApiBase::AuvState AuvRpcLibClient::getAuvState(const std::string& vehicle_name)
        {
            return static_cast<rpc::client*>(getClient())->call("getAuvState", vehicle_name).as<AuvRpcLibAdaptors::AuvState>().to();
        }
        AuvApiBase::AuvControls AuvRpcLibClient::getAuvControls(const std::string& vehicle_name)
        {
            return static_cast<rpc::client*>(getClient())->call("getAuvControls", vehicle_name).as<AuvRpcLibAdaptors::AuvControls>().to();
        }

        void AuvRpcLibClient::setOceanCurrent(float vx_north, float vy_east, float vz_down, const std::string& vehicle_name)
        {
            Vector3r current(vx_north, vy_east, vz_down);
            AuvRpcLibAdaptors::Vector3r conv_current(current);
            static_cast<rpc::client*>(getClient())->call("setOceanCurrent", conv_current, vehicle_name);
        }
    }
} //namespace

#endif
#endif

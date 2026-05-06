// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AuvRpcLibClient_hpp
#define air_AuvRpcLibClient_hpp

#include "common/Common.hpp"
#include <functional>
#include "common/CommonStructs.hpp"
#include "vehicles/auv/api/AuvApiBase.hpp"
#include "api/RpcLibClientBase.hpp"
#include "common/ImageCaptureBase.hpp"

namespace msr
{
namespace airlib
{

    class AuvRpcLibClient : public RpcLibClientBase
    {
    public:
        AuvRpcLibClient(const string& ip_address = "localhost", uint16_t port = RpcLibPort, float timeout_sec = 60);


        // AuvRpcLibClient* moveByVelocityAsync(float vx, float vy, float vz, float yaw, float duration,
        //                                             const std::string& vehicle_name = "");

        // void setAuvControls(const AuvApiBase::AuvControls& controls, const std::string& vehicle_name = "");
        void setAuvControls(float fx, float fy, float fz,
                            float roll, float pitch, float yaw,
                            const std::string& vehicle_name = "");
        AuvApiBase::AuvState getAuvState(const std::string& vehicle_name = "");
        AuvApiBase::AuvControls getAuvControls(const std::string& vehicle_name = "");

        // Set ocean current velocity in NED world frame [m/s].
        void setOceanCurrent(float vx_north, float vy_east, float vz_down, const std::string& vehicle_name = "");
        virtual ~AuvRpcLibClient(); //required for pimpl
    };
}
} //namespace
#endif

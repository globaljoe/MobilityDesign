// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AuvRpcLibServer_hpp
#define air_AuvRpcLibServer_hpp

#ifndef AIRLIB_NO_RPC

#include "common/Common.hpp"
#include <functional>
#include "api/RpcLibServerBase.hpp"
#include "vehicles/auv/api/AuvApiBase.hpp"

namespace msr
{
namespace airlib
{

    class AuvRpcLibServer : public RpcLibServerBase
    {
    public:
        AuvRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port = RpcLibPort);
        virtual ~AuvRpcLibServer();

    protected:
        virtual AuvApiBase* getVehicleApi(const std::string& vehicle_name) override
        {
            return static_cast<AuvApiBase*>(RpcLibServerBase::getVehicleApi(vehicle_name));
        }
    };

#endif
}
} //namespace
#endif
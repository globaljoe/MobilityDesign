// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AsvRpcLibServer_hpp
#define air_AsvRpcLibServer_hpp

#ifndef AIRLIB_NO_RPC

#include "common/Common.hpp"
#include <functional>
#include "api/RpcLibServerBase.hpp"
#include "vehicles/asv/api/AsvApiBase.hpp"

namespace msr
{
namespace airlib
{

    class AsvRpcLibServer : public RpcLibServerBase
    {
    public:
        AsvRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port = RpcLibPort);
        virtual ~AsvRpcLibServer();

    protected:
        virtual AsvApiBase* getVehicleApi(const std::string& vehicle_name) override
        {
            return static_cast<AsvApiBase*>(RpcLibServerBase::getVehicleApi(vehicle_name));
        }
    };

#endif
}
} //namespace
#endif
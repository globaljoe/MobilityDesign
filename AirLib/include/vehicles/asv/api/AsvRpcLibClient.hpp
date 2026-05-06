// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AsvRpcLibClient_hpp
#define air_AsvRpcLibClient_hpp

#include "common/Common.hpp"
#include <functional>
#include "common/CommonStructs.hpp"
#include "vehicles/asv/api/AsvApiBase.hpp"
#include "api/RpcLibClientBase.hpp"
#include "common/ImageCaptureBase.hpp"

namespace msr
{
namespace airlib
{

    class AsvRpcLibClient : public RpcLibClientBase
    {
    public:
        AsvRpcLibClient(const string& ip_address = "localhost", uint16_t port = RpcLibPort, float timeout_sec = 60);

        void setAsvControls(const AsvApiBase::AsvControls& controls, const std::string& vehicle_name = "");
        AsvApiBase::AsvState getAsvState(const std::string& vehicle_name = "");
        AsvApiBase::AsvControls getAsvControls(const std::string& vehicle_name = "");
        virtual ~AsvRpcLibClient(); //required for pimpl
    };
}
} //namespace
#endif

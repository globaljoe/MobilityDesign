// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_AsvApiFactory_hpp
#define msr_airlib_vehicles_AsvApiFactory_hpp

#include "vehicles/asv/firmwares/asv_simple/AsvSimpleApi.hpp"
// #include "vehicles/asv/firmwares/ardurover/ArduRoverApi.hpp"

namespace msr
{
namespace airlib
{

    class AsvApiFactory
    {
    public:
        static std::unique_ptr<AsvApiBase> createApi(const AirSimSettings::VehicleSetting* vehicle_setting,
                                                     std::shared_ptr<SensorFactory> sensor_factory,
                                                     const Kinematics::State& state, const Environment& environment)
        {
            // if (vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeArduRover) {
            //     return std::unique_ptr<AsvApiBase>(new ArduRoverApi(vehicle_setting, sensor_factory, state, environment));
            // } else
            if (vehicle_setting->vehicle_type == "" || //default config
                     vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeAsvSimple) {
                return std::unique_ptr<AsvApiBase>(new AsvSimpleApi(vehicle_setting, sensor_factory, state, environment));
            }
            else
                throw std::runtime_error(Utils::stringf(
                    "Cannot create vehicle config because vehicle name '%s' is not recognized",
                    vehicle_setting->vehicle_name.c_str()));
        }
    };
}
} // namespace

#endif

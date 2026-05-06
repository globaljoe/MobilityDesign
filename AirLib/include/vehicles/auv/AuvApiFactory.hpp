// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_vehicles_AuvApiFactory_hpp
#define msr_airlib_vehicles_AuvApiFactory_hpp

#include "vehicles/auv/firmwares/auv_simple/AuvSimpleApi.hpp"
// #include "vehicles/auv/firmwares/ardurover/ArduRoverApi.hpp"

namespace msr
{
namespace airlib
{

    class AuvApiFactory
    {
    public:
        static std::unique_ptr<AuvApiBase> createApi(const AirSimSettings::VehicleSetting* vehicle_setting,
                                                     std::shared_ptr<SensorFactory> sensor_factory,
                                                     const Kinematics::State& state, const Environment& environment)
        {
            // if (vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeArduRover) {
            //     return std::unique_ptr<AuvApiBase>(new ArduRoverApi(vehicle_setting, sensor_factory, state, environment));
            // } else
            if (vehicle_setting->vehicle_type == "" || //default config
                     vehicle_setting->vehicle_type == AirSimSettings::kVehicleTypeAuvSimple) {
                return std::unique_ptr<AuvApiBase>(new AuvSimpleApi(vehicle_setting, sensor_factory, state, environment));
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

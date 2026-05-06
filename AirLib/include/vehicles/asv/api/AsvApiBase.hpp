// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AsvApiBase_hpp
#define air_AsvApiBase_hpp

#include "common/VectorMath.hpp"
#include "AsvCommon.hpp"
#include "common/CommonStructs.hpp"
#include "vehicles/asv/AsvParams.hpp"
#include "api/VehicleApiBase.hpp"
#include "physics/Kinematics.hpp"
#include "sensors/SensorBase.hpp"
#include "sensors/SensorCollection.hpp"
#include "sensors/SensorFactory.hpp"

namespace msr
{
namespace airlib
{

    class AsvApiBase : public VehicleApiBase
    {
    public:
         struct AsvControls
        {
            float left_force  = 0.f;  // [N]  port  thruster force, positive = forward
            float right_force = 0.f;  // [N]  starboard thruster force, positive = forward

            AsvControls() {}
            AsvControls(float left_force_val, float right_force_val)
                : left_force(left_force_val), right_force(right_force_val) {}
        };

        struct AsvState
        {
            Kinematics::State kinematics_estimated;
            uint64_t timestamp;

            AsvState()
            {
            }

            AsvState(const Kinematics::State& kinematics_estimated_val, uint64_t timestamp_val)
                : kinematics_estimated(kinematics_estimated_val), timestamp(timestamp_val)
            {
            }

            //shortcuts
            const Vector3r& getPosition() const
            {
                return kinematics_estimated.pose.position;
            }
            const Quaternionr& getOrientation() const
            {
                return kinematics_estimated.pose.orientation;
            }
        };

    public:
        // TODO: Temporary constructor for the Unity implementation which does not use the new Sensor Configuration Settings implementation.
        //AsvApiBase() {}

        AsvApiBase(const AirSimSettings::VehicleSetting* vehicle_setting,
                   std::shared_ptr<SensorFactory> sensor_factory,
                   const Kinematics::State& state, const Environment& environment)
        {
            initialize(vehicle_setting, sensor_factory, state, environment);
        }

        virtual void update() override
        {
            VehicleApiBase::update();

            getSensors().update();
        }

        void reportState(StateReporter& reporter) override
        {
            getSensors().reportState(reporter);
        }

        // sensor helpers
        virtual const SensorCollection& getSensors() const override
        {
            return sensors_;
        }

        SensorCollection& getSensors()
        {
            return sensors_;
        }

        void initialize(const AirSimSettings::VehicleSetting* vehicle_setting,
                        std::shared_ptr<SensorFactory> sensor_factory,
                        const Kinematics::State& state, const Environment& environment)
        {
            sensor_factory_ = sensor_factory;

            sensor_storage_.clear();
            sensors_.clear();

            addSensorsFromSettings(vehicle_setting);

            getSensors().initialize(&state, &environment);
        }

        void addSensorsFromSettings(const AirSimSettings::VehicleSetting* vehicle_setting)
        {
            const auto& sensor_settings = vehicle_setting->sensors;

            sensor_factory_->createSensorsFromSettings(sensor_settings, sensors_, sensor_storage_);
        }

        virtual void setAsvControls(const AsvControls& controls) = 0;
        virtual void updateAsvState(const AsvState& state) = 0;
        virtual const AsvState& getAsvState() const = 0;
        virtual const AsvControls& getAsvControls() const = 0;
        virtual const AsvVehicleParams& getVehicleParams() const = 0;

        virtual ~AsvApiBase() = default;

        std::shared_ptr<const SensorFactory> sensor_factory_;
        SensorCollection sensors_; //maintains sensor type indexed collection of sensors
        vector<shared_ptr<SensorBase>> sensor_storage_; //RAII for created sensors

    protected:
        virtual void resetImplementation() override
        {
            //reset sensors last after their ground truth has been reset
            getSensors().reset();
        }
    
    private: //variables
        // ThrusterStates thruster_states_;
    };
}
} //namespace
#endif

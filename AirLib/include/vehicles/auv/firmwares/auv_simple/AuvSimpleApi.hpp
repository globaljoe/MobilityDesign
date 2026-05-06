// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_AuvSimpleController_hpp
#define msr_airlib_AuvSimpleController_hpp

#include "vehicles/auv/api/AuvApiBase.hpp"

namespace msr
{
namespace airlib
{

    class AuvSimpleApi : public AuvApiBase
    {
    public:
        AuvSimpleApi(const AirSimSettings::VehicleSetting* vehicle_setting, std::shared_ptr<SensorFactory> sensor_factory,
                    const Kinematics::State& state, const Environment& environment)
            : AuvApiBase(vehicle_setting, sensor_factory, state, environment), home_geopoint_(environment.getHomeGeoPoint())
        {
        }

        ~AuvSimpleApi()
        {
        }

    protected:
        virtual void resetImplementation() override
        {
            AuvApiBase::resetImplementation();
        }

    public:
        virtual void update() override
        {
            AuvApiBase::update();
        }

        virtual const SensorCollection& getSensors() const override
        {
            return AuvApiBase::getSensors();
        }

        // VehicleApiBase Implementation
        virtual void enableApiControl(bool is_enabled) override
        {
            if (api_control_enabled_ != is_enabled) {
                last_controls_ = AuvControls();
                api_control_enabled_ = is_enabled;
            }
        }

        virtual bool isApiControlEnabled() const override
        {
            return api_control_enabled_;
        }

        virtual GeoPoint getHomeGeoPoint() const override
        {
            return home_geopoint_;
        }

        virtual bool armDisarm(bool arm) override
        {
            //TODO: implement arming for auv
            unused(arm);
            return true;
        }

    public:
        virtual const AuvVehicleParams& getVehicleParams() const override
        {
            return params_;
        }

        virtual void setAuvControls(const AuvControls& controls) override
        {
            last_controls_ = controls;
        }

        virtual void updateAuvState(const AuvState& auv_state) override
        {
            last_auv_state_ = auv_state;
        }

        virtual const AuvState& getAuvState() const override
        {
            return last_auv_state_;
        }

        virtual const AuvControls& getAuvControls() const override
        {
            return last_controls_;
        }

        virtual void setOceanCurrent(const Vector3r& current) override
        {
            ocean_current_ = current;
        }

        virtual Vector3r getOceanCurrent() const override
        {
            return ocean_current_;
        }

    private:
        bool api_control_enabled_ = false;
        GeoPoint home_geopoint_;
        AuvControls last_controls_;
        AuvState last_auv_state_;
        AuvVehicleParams params_;                    // default values defined in AuvParams.hpp
        Vector3r ocean_current_ = Vector3r::Zero();  // NED world frame [m/s]
    };
}
}

#endif

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_DvlSimpleParams_hpp
#define msr_airlib_DvlSimpleParams_hpp

#include "common/Common.hpp"
#include "common/EarthUtils.hpp"
#include "common/AirSimSettings.hpp"
#include <cmath>

namespace msr
{
namespace airlib
{

    struct DvlSimpleParams
    {
        //see PX4 param reference for EKF: https://dev.px4.io/en/advanced/parameter_reference.html
        real_T update_latency = 0.0f; //sec
        real_T update_frequency = 4; //Hz
        real_T startup_delay = 0; //sec

        struct Velocimeter
        {
            //velocity random walk (VRW)
            real_T vrw = 0.05f; //[m/s] 0.05f, 6.0f * 1E-6f
            // real_T linear_velocity_noise_sigma = 0.05f;
            // real_T angular_velocity_noise_sigma = 0.10f;
            Vector3r turn_on_bias = Vector3r::Zero(); //assume calibration is done
            //Bias Stability (tau = 800s)
            real_T tau = 100;
            real_T bias_stability = 36.0f * 1E-3f; //ug converted to m/s^2 -> 36.0f * 1E-6f;
            // 36.0f * 1E-6f,  6.0f * 1E-6f
        } vel;
        
        struct Altimeter
        {
            real_T min_distance = 20.0f / 100; //m
            real_T max_distance = 4000.0f / 100; //m

            Pose relative_pose{
                Vector3r(0, 0, -1), // position - a little above vehicle (especially for cars) or Vector3r::Zero()
                Quaternionr::Identity() // orientation - by default Quaternionr(1, 0, 0, 0)
            };

            bool draw_debug_points = false;
            bool external_controller = true;

            /*
            Ref: A Stochastic Approach to Noise Modeling for Barometric Altimeters
            Angelo Maria Sabatini* and Vincenzo Genovese
            Sample values are from Table 1
            https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3871085/
            This is however not used because numbers mentioned in paper doesn't match experiments.

            real_T correlated_noise_sigma = 0.27f;
            real_T correlated_noise_tau = 0.87f;
            real_T uncorrelated_noise_sigma = 0.24f;

            */
            //TODO: update sigma based on documentation, maybe as a function increasing with measured distance
            real_T uncorrelated_noise_sigma = 0.002f * 100;
            //jMavSim uses below
            //real_T uncorrelated_noise_sigma = 0.1f;
        } alti;

        //For built-in imu and estimated pose
        struct Gyroscope
        {
            //angular random walk (ARW)
            real_T arw = 0.15f / sqrt(1.0f) * M_PIf / 180; //deg/sqrt(hour) converted to rad/sqrt(sec)
            //Bias Stability (tau = 500s)
            real_T tau = 500;
            real_T bias_stability = 10.0f / 100 * M_PIf / 180; //deg/hr converted to rad/sec -> 4.6f / 100 * M_PIf / 180;
            Vector3r turn_on_bias = Vector3r::Zero(); //assume calibration is done
        } gyro;


        void initializeFromSettings(const AirSimSettings::DvlSetting& settings)
        {
            const auto& settings_json = settings.settings;
            alti.min_distance = settings_json.getFloat("MinDistance", alti.min_distance);
            alti.max_distance = settings_json.getFloat("MaxDistance", alti.max_distance);
            alti.uncorrelated_noise_sigma = settings_json.getFloat("NoiseSigma", alti.uncorrelated_noise_sigma);
            alti.draw_debug_points = settings_json.getBool("DrawDebugPoints", alti.draw_debug_points);
            alti.external_controller = settings_json.getBool("ExternalController", alti.external_controller);

            auto position = AirSimSettings::createVectorSetting(settings_json, VectorMath::nanVector());
            auto rotation = AirSimSettings::createRotationSetting(settings_json, AirSimSettings::Rotation::nanRotation());

            std::string simmode_name = AirSimSettings::singleton().simmode_name;

            alti.relative_pose.position = position;
            if (std::isnan(alti.relative_pose.position.x()))
                alti.relative_pose.position.x() = 0;
            if (std::isnan(alti.relative_pose.position.y()))
                alti.relative_pose.position.y() = 0;
            if (std::isnan(alti.relative_pose.position.z())) {
                if (simmode_name == AirSimSettings::kSimModeTypeMultirotor)
                    alti.relative_pose.position.z() = 0;
                else
                    alti.relative_pose.position.z() = -1; // a little bit above for cars
            }

            float pitch, roll, yaw;
            pitch = !std::isnan(rotation.pitch) ? rotation.pitch : -90;
            roll = !std::isnan(rotation.roll) ? rotation.roll : 0;
            yaw = !std::isnan(rotation.yaw) ? rotation.yaw : 0;
            alti.relative_pose.orientation = VectorMath::toQuaternion(
                Utils::degreesToRadians(pitch), //pitch - rotation around Y axis
                Utils::degreesToRadians(roll), //roll  - rotation around X axis
                Utils::degreesToRadians(yaw)); //yaw   - rotation around Z axis
        }
    };
}
} //namespace
#endif

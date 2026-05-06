// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_GpuSidescanSonarBase_hpp
#define msr_airlib_GpuSidescanSonarBase_hpp

#include "sensors/SensorBase.hpp"

namespace msr
{
namespace airlib
{

    class GpuSidescanSonarBase : public SensorBase
    {
    public:
        GpuSidescanSonarBase(const std::string& sensor_name = "")
            : SensorBase(sensor_name)
        {
        }

    public:
        virtual void reportState(StateReporter& reporter) override
        {
            //call base
            UpdatableObject::reportState(reporter);

            reporter.writeValue("GpuSidescanSonar-Timestamp", output_.time_stamp);
            // reporter.writeValue("GpuSidescanSonar-NumPoints", static_cast<int>(output_.port_point_cloud.size() / 3));

        }

        const GpuSidescanSonarData& getOutput() const
        {
            return output_;
        }

    protected:
        void setOutput(const GpuSidescanSonarData& output)
        {
            output_ = output;
        }

    private:
        GpuSidescanSonarData output_;
    };
}
} //namespace
#endif

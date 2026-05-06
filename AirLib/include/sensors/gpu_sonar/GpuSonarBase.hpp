// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_GpuSonarBase_hpp
#define msr_airlib_GpuSonarBase_hpp

#include "sensors/SensorBase.hpp"

namespace msr
{
namespace airlib
{

    class GpuSonarBase : public SensorBase
    {
    public:
        GpuSonarBase(const std::string& sensor_name = "")
            : SensorBase(sensor_name)
        {
        }

    public:
        virtual void reportState(StateReporter& reporter) override
        {
            //call base
            UpdatableObject::reportState(reporter);

            reporter.writeValue("GpuSonar-Timestamp", output_.time_stamp);
            // reporter.writeValue("GpuSonar-NumPoints", static_cast<int>(output_.sonar_raw_data.size() / 3));

        }

        const GpuSonarData& getOutput() const
        {
            return output_;
        }

    protected:
        void setOutput(const GpuSonarData& output)
        {
            output_ = output;
        }

    private:
        GpuSonarData output_;
    };
}
} //namespace
#endif

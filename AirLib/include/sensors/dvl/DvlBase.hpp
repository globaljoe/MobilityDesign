// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_DvlBase_hpp
#define msr_airlib_DvlBase_hpp

#include "sensors/SensorBase.hpp"

namespace msr
{
namespace airlib
{

    class DvlBase : public SensorBase
    {
    public:
        DvlBase(const std::string& sensor_name = "")
            : SensorBase(sensor_name)
        {
        }

    public:
        virtual void reportState(StateReporter& reporter) override
        {
            //call base
            UpdatableObject::reportState(reporter);

            reporter.writeValue("DVL-Timestamp", output_.time_stamp);
            reporter.writeValue("DVL-Vel", output_.velocity);
            reporter.writeValue("DVL-Alti", output_.altitude);
            reporter.writeValue("DVL-Position", output_.estimated_pose.position);
            reporter.writeValue("DVL-Orientation", output_.estimated_pose.orientation);
        }

        const DvlData& getOutput() const
        {
            return output_;
        }

    protected:
        void setOutput(const DvlData& output)
        {
            output_ = output;
        }

    private:
        DvlData output_;
    };
}
} //namespace
#endif

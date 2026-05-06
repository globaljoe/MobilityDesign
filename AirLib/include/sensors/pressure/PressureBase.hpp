// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef msr_airlib_PressureBase_hpp
#define msr_airlib_PressureBase_hpp

#include "sensors/SensorBase.hpp"

namespace msr
{
namespace airlib
{

    class PressureBase : public SensorBase
    {
    public:
        PressureBase(const std::string& sensor_name = "")
            : SensorBase(sensor_name)
        {
        }

    public: //types
        struct Output
        { //same fields as ROS message
            TTimePoint time_stamp;
            real_T depth; //meters
            real_T pressure; //Pascal
        };

    public:
        virtual void reportState(StateReporter& reporter) override
        {
            //call base
            UpdatableObject::reportState(reporter);

            reporter.writeValue("Prs-Dep", output_.depth);
            reporter.writeValue("Prs-Prs", output_.pressure);
        }

        const Output& getOutput() const
        {
            return output_;
        }

    protected:
        void setOutput(const Output& output)
        {
            output_ = output;
        }

    private:
        Output output_;
    };
}
} //namespace
#endif

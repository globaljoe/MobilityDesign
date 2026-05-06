import setup_path
import furosim

import numpy as np
import os
import time
import pprint

# connect to the AirSim simulator
client = furosim.AuvClient()
client.confirmConnection()
client.enableApiControl(True)
print("API Control enabled: %s" % client.isApiControlEnabled())

state = client.getAuvState()
s = pprint.pformat(state)
print("state: %s" % s)

imu_data = client.getImuData()
s = pprint.pformat(imu_data)
print("imu_data: %s" % s)

dvl_data = client.getDvlData()
s = pprint.pformat(dvl_data)
print("dvl_data: %s" % s)

pressure_data = client.getPressureData()
s = pprint.pformat(pressure_data)
print("pressure_data: %s" % s)

furosim.wait_key('Press any key to apply forward thrust for 3 seconds')
auv_controls = furosim.AuvControls()

# go forward
auv_controls.force = [50.0, 0.0, 0.0]   # x, y, z forces in Newtons
auv_controls.torque = [0.0, 0.0, 0.0]  # roll, pitch, yaw torques in Newton-meters
client.setAuvControls(auv_controls)
print("Go Forward")
time.sleep(3)

# remove thrust (vehicle decelerates by hydrodynamic drag)
auv_controls.force = [0.0, 0.0, 0.0]
client.setAuvControls(auv_controls)
print("Thrust off")
time.sleep(3)

furosim.wait_key('Press any key to yaw right for 3 seconds')

# yaw right
auv_controls.force  = [0.0, 0.0, 0.0]
auv_controls.torque = [0.0, 0.0, 2.0]  # positive yaw = clockwise (NED)
client.setAuvControls(auv_controls)
print("Yaw Right")
time.sleep(3)

# remove torque (vehicle stops rotating by hydrodynamic drag)
auv_controls.torque = [0.0, 0.0, 0.0]
client.setAuvControls(auv_controls)
print("Torque off")
time.sleep(3)
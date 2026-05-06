import setup_path
import furosim

import time

# connect to the AirSim simulator
client = furosim.AuvClient()
client.confirmConnection()
client.enableApiControl(True)
print("API Control enabled: %s" % client.isApiControlEnabled())

# Set ocean current in NED world frame [vx_north, vy_east, vz_down] (m/s).
# The current persists until changed or the simulation is reset.
furosim.wait_key('Press any key to apply ocean current (1.0 m/s northward)')
client.setOceanCurrent(1.0, 0.0, 0.0)
print("Ocean current applied: 1.0 m/s northward")

time.sleep(10)

# Remove ocean current
client.setOceanCurrent(0.0, 0.0, 0.0)
print("Ocean current removed")
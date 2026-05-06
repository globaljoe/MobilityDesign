# In settings.json first activate computer vision mode: 
# https://github.com/Microsoft/AirSim/blob/main/docs/image_apis.md#computer-vision-mode

import setup_path 
import furosim

import pprint
import tempfile
import os
import time

pp = pprint.PrettyPrinter(indent=4)

client = furosim.VehicleClient()

furosim.wait_key('Press any key to get camera parameters')
for camera_id in range(2):
    camera_info = client.simGetCameraInfo(str(camera_id))
    print("CameraInfo %d: %s" % (camera_id, pp.pprint(camera_info)))

furosim.wait_key('Press any key to get images')
tmp_dir = os.path.join(tempfile.gettempdir(), "airsim_drone")
print ("Saving images to %s" % tmp_dir)
try:
    for n in range(3):
        os.makedirs(os.path.join(tmp_dir, str(n)))
except OSError:
    if not os.path.isdir(tmp_dir):
        raise

for x in range(50): # do few times
    #xn = 1 + x*5  # some random number
    client.simSetVehiclePose(furosim.Pose(furosim.Vector3r(x, 0, -2), furosim.to_quaternion(0, 0, 0)), True)
    time.sleep(0.1)

    responses = client.simGetImages([
        furosim.ImageRequest("0", furosim.ImageType.Scene),
        furosim.ImageRequest("1", furosim.ImageType.Scene),
        furosim.ImageRequest("2", furosim.ImageType.Scene)])

    for i, response in enumerate(responses):
        if response.pixels_as_float:
            print("Type %d, size %d, pos %s" % (response.image_type, len(response.image_data_float), pprint.pformat(response.camera_position)))
            furosim.write_pfm(os.path.normpath(os.path.join(tmp_dir, str(x) + "_" + str(i) + '.pfm')), furosim.get_pfm_array(response))
        else:
            print("Type %d, size %d, pos %s" % (response.image_type, len(response.image_data_uint8), pprint.pformat(response.camera_position)))
            furosim.write_file(os.path.normpath(os.path.join(tmp_dir, str(i), str(x) + "_" + str(i) + '.png')), response.image_data_uint8)

    pose = client.simGetVehiclePose()
    pp.pprint(pose)

    time.sleep(3)

# currently reset() doesn't work in CV mode. Below is the workaround
client.simSetVehiclePose(furosim.Pose(furosim.Vector3r(0, 0, 0), furosim.to_quaternion(0, 0, 0)), True)

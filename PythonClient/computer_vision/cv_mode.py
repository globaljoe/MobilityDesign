# In settings.json first activate computer vision mode:
# https://github.com/Microsoft/AirSim/blob/main/docs/image_apis.md#computer-vision-mode

import setup_path
import furosim

import pprint
import os
import time
import math
import tempfile

pp = pprint.PrettyPrinter(indent=4)

client = furosim.VehicleClient()
client.confirmConnection()

furosim.wait_key('Press any key to set camera-0 gimbal to 15-degree pitch')
camera_pose = furosim.Pose(furosim.Vector3r(0, 0, 0), furosim.to_quaternion(math.radians(15), 0, 0)) #radians
client.simSetCameraPose("0", camera_pose)

furosim.wait_key('Press any key to get camera parameters')
for camera_name in range(5):
    camera_info = client.simGetCameraInfo(str(camera_name))
    print("CameraInfo %d:" % camera_name)
    pp.pprint(camera_info)

tmp_dir = os.path.join(tempfile.gettempdir(), "airsim_cv_mode")
print ("Saving images to %s" % tmp_dir)
try:
    os.makedirs(tmp_dir)
except OSError:
    if not os.path.isdir(tmp_dir):
        raise

furosim.wait_key('Press any key to get images')
for x in range(3): # do few times
    z = x * -20 - 5 # some random number
    client.simSetVehiclePose(furosim.Pose(furosim.Vector3r(z, z, z), furosim.to_quaternion(x / 3.0, 0, x / 3.0)), True)

    responses = client.simGetImages([
        furosim.ImageRequest("0", furosim.ImageType.DepthVis),
        furosim.ImageRequest("1", furosim.ImageType.DepthPerspective, True),
        furosim.ImageRequest("2", furosim.ImageType.Segmentation),
        furosim.ImageRequest("3", furosim.ImageType.Scene),
        furosim.ImageRequest("4", furosim.ImageType.DisparityNormalized),
        furosim.ImageRequest("4", furosim.ImageType.SurfaceNormals)])

    for i, response in enumerate(responses):
        filename = os.path.join(tmp_dir, str(x) + "_" + str(i))
        if response.pixels_as_float:
            print("Type %d, size %d, pos %s" % (response.image_type, len(response.image_data_float), pprint.pformat(response.camera_position)))
            furosim.write_pfm(os.path.normpath(filename + '.pfm'), furosim.get_pfm_array(response))
        else:
            print("Type %d, size %d, pos %s" % (response.image_type, len(response.image_data_uint8), pprint.pformat(response.camera_position)))
            furosim.write_file(os.path.normpath(filename + '.png'), response.image_data_uint8)

    pose = client.simGetVehiclePose()
    pp.pprint(pose)

    time.sleep(3)

# currently reset() doesn't work in CV mode. Below is the workaround
client.simSetVehiclePose(furosim.Pose(furosim.Vector3r(0, 0, 0), furosim.to_quaternion(0, 0, 0)), True)

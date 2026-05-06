import setup_path
import furosim
import cv2
import json
import numpy as np


# connect to the AirSim simulator
client = furosim.VehicleClient()
client.confirmConnection()

# set vehicle and sonar names
# sonar_name must match the sensor name defined in settings.json
# e.g., if settings.json has "Sensors": { "didson": { "SensorType": 7, ... } }, set sonar_name = "didson"
# auv_with_didson.json -> sonar_name = "didson"
# auv_with_oculus.json -> sonar_name = "oculus"
vehicle_name = "AuvSimple"
sonar_name = "GSonar"
save_to_disk = False
save_dir = "sonar_logs"

settings_text = client.getSettingsString()
if not settings_text:
    raise ValueError("AirSim settings are empty.")

settings = json.loads(settings_text)
sensor_settings = settings["Vehicles"][vehicle_name]["Sensors"][sonar_name]
sonar_image_shape = (
    int(sensor_settings["NumberOfSamples"]),
    int(sensor_settings["NumberOfBeams"]),
)

if save_to_disk:
    import os
    os.makedirs(save_dir, exist_ok=True)


while True:
    sonar_data = client.getGpuSonarData(sonar_name, vehicle_name)
    if np.asarray(sonar_data.sonar_raw_data).size < 1:
        print("\tNo data received from Sonar")
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            break
        continue

    sonar_raw_data = np.array(sonar_data.sonar_raw_data, dtype=np.float32).reshape(sonar_image_shape)
    img = (255 * sonar_raw_data).astype(np.uint8)
    img_resized = cv2.resize(
        img,
        (img.shape[1] * 2, img.shape[0] * 2),
        interpolation=cv2.INTER_NEAREST,
    )

    cv2.imshow("GSonar Image", img_resized)

    if save_to_disk:
        prefix = f"{save_dir}/{sonar_data.time_stamp}_img.png"
        cv2.imwrite(prefix, img)
        print(f"\t{prefix} has been saved")

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break


cv2.destroyAllWindows()

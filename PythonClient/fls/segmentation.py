import setup_path
import furosim
import ast
import cv2
import json
import numpy as np
from pathlib import Path


# connect to the AirSim simulator
client = furosim.VehicleClient()
client.confirmConnection()

# set vehicle and sonar names
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

color_map = {}
with open(Path(__file__).resolve().parents[2] / "docs" / "seg_rgbs.txt", "r", encoding="utf-8") as f:
    for line in f:
        line = line.strip()
        if not line:
            continue
        idx, rgb = line.split("\t")
        color_map[int(idx)] = ast.literal_eval(rgb)

lut = np.zeros((max(color_map) + 1, 3), dtype=np.uint8)
for idx, rgb in color_map.items():
    lut[idx] = rgb

if save_to_disk:
    import os
    os.makedirs(save_dir, exist_ok=True)


while True:
    sonar_data = client.getGpuSonarData(sonar_name, vehicle_name)
    if np.asarray(sonar_data.point_cloud).size < 3:
        print("\tNo points received from Sonar data")
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            break
        continue

    seg_img = np.array(sonar_data.segmentation_sonar, dtype=np.float32).reshape(sonar_image_shape).astype(np.uint8)
    seg_img_color = lut[seg_img]

    seg_img_resized = cv2.resize(
        seg_img,
        (seg_img.shape[1] * 2, seg_img.shape[0] * 2),
        interpolation=cv2.INTER_NEAREST,
    )
    seg_img_color_resized = cv2.resize(
        seg_img_color,
        (seg_img_color.shape[1] * 2, seg_img_color.shape[0] * 2),
        interpolation=cv2.INTER_NEAREST,
    )

    cv2.imshow("GSonar LabelIds", seg_img_resized)
    cv2.imshow("GSonar Segmentation", seg_img_color_resized)

    if save_to_disk:
        label_prefix = f"{save_dir}/{sonar_data.time_stamp}_labelIds.png"
        color_prefix = f"{save_dir}/{sonar_data.time_stamp}_color.png"
        cv2.imwrite(label_prefix, seg_img)
        cv2.imwrite(color_prefix, seg_img_color)
        print(f"\t{label_prefix} has been saved")

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break


cv2.destroyAllWindows()

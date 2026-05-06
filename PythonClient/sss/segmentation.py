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
sonar_name = "GSidescan"
save_to_disk = False
save_dir = "sonar_logs"
history_length = 600
window_scale = 1
use_orange_palette = True

settings_text = client.getSettingsString()
if not settings_text:
    raise ValueError("AirSim settings are empty.")

settings = json.loads(settings_text)
sensor_settings = settings["Vehicles"][vehicle_name]["Sensors"][sonar_name]
sonar_image_shape = (
    1,
    int(sensor_settings["NumberOfSamples"]),
)

if save_to_disk:
    import os
    os.makedirs(save_dir, exist_ok=True)

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

orange_palette = np.zeros((256, 1, 3), dtype=np.uint8)
palette_anchors = [
    (0, np.array([0, 0, 0], dtype=np.float32)),
    (32, np.array([12, 22, 45], dtype=np.float32)),
    (96, np.array([20, 60, 120], dtype=np.float32)),
    (160, np.array([30, 110, 190], dtype=np.float32)),
    (224, np.array([60, 170, 240], dtype=np.float32)),
    (255, np.array([120, 220, 255], dtype=np.float32)),
]
for i in range(len(palette_anchors) - 1):
    start_idx, start_color = palette_anchors[i]
    end_idx, end_color = palette_anchors[i + 1]
    span = end_idx - start_idx
    for j in range(span + 1):
        t = 0.0 if span == 0 else j / span
        orange_palette[start_idx + j, 0] = np.round((1.0 - t) * start_color + t * end_color).astype(np.uint8)

waterfall = np.zeros((history_length, sonar_image_shape[1]), dtype=np.uint8)
waterfall_starboard = np.zeros((history_length, sonar_image_shape[1]), dtype=np.uint8)
seg_waterfall = np.zeros((history_length, sonar_image_shape[1]), dtype=np.uint8)
seg_waterfall_starboard = np.zeros((history_length, sonar_image_shape[1]), dtype=np.uint8)


while True:
    sonar_data = client.getGpuSidescanSonarData(sonar_name, vehicle_name)
    if (
        np.asarray(sonar_data.port_raw_data).size < sonar_image_shape[1]
        or np.asarray(sonar_data.starboard_raw_data).size < sonar_image_shape[1]
        or np.asarray(sonar_data.port_segmentation).size < sonar_image_shape[1]
        or np.asarray(sonar_data.starboard_segmentation).size < sonar_image_shape[1]
    ):
        print("\tNo sidescan samples received")
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q'):
            break
        continue

    port_raw_data = np.array(sonar_data.port_raw_data, dtype=np.float32).reshape(sonar_image_shape)
    starboard_raw_data = np.array(sonar_data.starboard_raw_data, dtype=np.float32).reshape(sonar_image_shape)
    port_segmentation = np.array(sonar_data.port_segmentation, dtype=np.int32).reshape(sonar_image_shape)
    starboard_segmentation = np.array(sonar_data.starboard_segmentation, dtype=np.int32).reshape(sonar_image_shape)

    row = (255 * port_raw_data).astype(np.uint8)[:, ::-1]
    row_starboard = (255 * starboard_raw_data).astype(np.uint8)
    seg_row = port_segmentation.astype(np.uint8)[:, ::-1]
    seg_row_starboard = starboard_segmentation.astype(np.uint8)

    waterfall = np.roll(waterfall, 1, axis=0)
    waterfall[0, :] = row[0]
    waterfall_starboard = np.roll(waterfall_starboard, 1, axis=0)
    waterfall_starboard[0, :] = row_starboard[0]
    seg_waterfall = np.roll(seg_waterfall, 1, axis=0)
    seg_waterfall[0, :] = seg_row[0]
    seg_waterfall_starboard = np.roll(seg_waterfall_starboard, 1, axis=0)
    seg_waterfall_starboard[0, :] = seg_row_starboard[0]

    full_waterfall = np.hstack((waterfall, waterfall_starboard))
    full_seg = np.hstack((seg_waterfall, seg_waterfall_starboard))

    img_resized = cv2.resize(
        full_waterfall,
        (full_waterfall.shape[1] * window_scale, full_waterfall.shape[0] * window_scale),
        interpolation=cv2.INTER_NEAREST,
    )
    seg_resized = cv2.resize(
        full_seg,
        (full_seg.shape[1] * window_scale, full_seg.shape[0] * window_scale),
        interpolation=cv2.INTER_NEAREST,
    )

    if use_orange_palette:
        img_colored = cv2.applyColorMap(img_resized, orange_palette)
    else:
        img_colored = cv2.cvtColor(img_resized, cv2.COLOR_GRAY2BGR)

    seg_max = int(seg_resized.max()) if seg_resized.size else 0
    if seg_max >= len(lut):
        lut = np.vstack((lut, np.zeros((seg_max + 1 - len(lut), 3), dtype=np.uint8)))
    seg_colored = lut[seg_resized]

    cv2.imshow("GSidescan Waterfall", img_colored)
    cv2.imshow("GSidescan Segmentation", seg_colored)

    if save_to_disk:
        prefix = f"{save_dir}/{sonar_data.time_stamp}_img.png"
        seg_prefix = f"{save_dir}/{sonar_data.time_stamp}_seg.png"
        cv2.imwrite(prefix, img_colored)
        cv2.imwrite(seg_prefix, seg_colored)
        print(f"\t{prefix} has been saved")

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break


cv2.destroyAllWindows()

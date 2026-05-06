import setup_path 
import furosim
import ast
import matplotlib.pyplot as plt
import numpy as np 
import pprint
from pathlib import Path

# connect to the AirSim simulator
client = furosim.VehicleClient()
client.confirmConnection()

# set sonar name and image type to request annotations and detections
sonar_name = "GSonar"
camera_name = sonar_name
image_type = furosim.ImageType.SegmentationID
mesh_name = "bricks*"

# set detection radius in [cm]
client.simSetDetectionFilterRadius(camera_name, image_type, 200 * 100) 
# add desired object name to detect in wild card/regex format
client.simAddDetectionFilterMeshName(camera_name, image_type, mesh_name) 

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

plt.ion()
figure = plt.figure(num=1)
axis = figure.add_subplot(111, projection="3d")
box_edges = [
    (0, 1), (1, 2), (2, 3), (3, 0),
    (4, 5), (5, 6), (6, 7), (7, 4),
    (0, 4), (1, 5), (2, 6), (3, 7),
]


while plt.fignum_exists(1):
    sonar_data = client.getGpuSonarData(sonar_name)
    detections = client.simGetDetections(camera_name, image_type)
    axis.cla()
    all_points = []

    point_cloud = np.array(sonar_data.point_cloud, dtype=np.float32).reshape(-1, 4)
    points = point_cloud[:, :3]
    labels = np.array(sonar_data.segmentation, dtype=np.int32).reshape(-1)
    point_count = min(points.shape[0], labels.shape[0])
    points = points[:point_count]
    labels = labels[:point_count]
    if labels.size:
        max_label = int(labels.max())
        if max_label >= len(lut):
            lut = np.vstack((lut, np.zeros((max_label + 1 - len(lut), 3), dtype=np.uint8)))
        point_colors = lut[labels].astype(np.float32) / 255.0
    else:
        point_colors = np.empty((0, 3), dtype=np.float32)

    point_step = max(points.shape[0] // 20000, 1)
    points = points[::point_step]
    point_colors = point_colors[::point_step]
    if points.size:
        axis.scatter(points[:, 0], points[:, 1], points[:, 2], c=point_colors, s=1, alpha=0.6, depthshade=False)
        all_points.append(points)

    if detections:
        for i, cylinder in enumerate(detections):
            s = pprint.pformat(cylinder)
            print("Cylinder: %s" % s)

            box_min = cylinder.box3D.min
            box_max = cylinder.box3D.max
            corners = np.array([
                [box_min.x_val, box_min.y_val, box_min.z_val],
                [box_max.x_val, box_min.y_val, box_min.z_val],
                [box_max.x_val, box_max.y_val, box_min.z_val],
                [box_min.x_val, box_max.y_val, box_min.z_val],
                [box_min.x_val, box_min.y_val, box_max.z_val],
                [box_max.x_val, box_min.y_val, box_max.z_val],
                [box_max.x_val, box_max.y_val, box_max.z_val],
                [box_min.x_val, box_max.y_val, box_max.z_val],
            ], dtype=np.float32)
            all_points.append(corners)
            color = plt.get_cmap("tab10")(i % 10)

            for start, end in box_edges:
                axis.plot(
                    [corners[start, 0], corners[end, 0]],
                    [corners[start, 1], corners[end, 1]],
                    [corners[start, 2], corners[end, 2]],
                    color=color,
                    linewidth=1.5,
                )

            center = corners.mean(axis=0)
            axis.text(center[0], center[1], center[2], cylinder.name, color=color)

    if all_points:
        stacked = np.vstack(all_points)
        axis.set_box_aspect(np.ptp(stacked, axis=0) + 1e-6)

    axis.set_xlabel("X")
    axis.set_ylabel("Y")
    axis.set_zlabel("Z")
    axis.set_title(sonar_name)
    plt.draw()
    plt.pause(0.001)

plt.close(figure)

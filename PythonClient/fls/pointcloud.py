import setup_path
import furosim
import matplotlib.pyplot as plt
import numpy as np


# connect to the AirSim simulator
client = furosim.VehicleClient()
client.confirmConnection()

# set vehicle and sonar names
sonar_name = "GSonar"

plt.ion()
figure = plt.figure(num=1)
axis = figure.add_subplot(111, projection="3d")


while plt.fignum_exists(1):
    sonar_data = client.getGpuSonarData(sonar_name)
    if not sonar_data.point_cloud:
        plt.pause(0.05)
        continue

    point_cloud = np.array(sonar_data.point_cloud, dtype=np.float32).reshape(-1, 4)
    points = point_cloud[:, :3]
    intensity = np.clip(point_cloud[:, 3], 0.0, 1.0)

    point_step = max(points.shape[0] // 20000, 1)
    points = points[::point_step]
    intensity = intensity[::point_step]

    axis.cla()
    axis.scatter(points[:, 0], points[:, 1], points[:, 2], c=intensity, cmap="gray", s=1, alpha=0.6, depthshade=False)
    axis.set_xlabel("X")
    axis.set_ylabel("Y")
    axis.set_zlabel("Z")
    axis.set_title(sonar_name)
    axis.set_box_aspect(np.ptp(points, axis=0) + 1e-6)
    plt.draw()
    plt.pause(0.001)


plt.close(figure)

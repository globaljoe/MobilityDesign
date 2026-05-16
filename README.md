# FURo-Sim (ROS 2 / Car teleoperation slice)

<p align="center">
  <a href="https://youtu.be/cEMKJDBZpxY">
    <img src="https://img.youtube.com/vi/cEMKJDBZpxY/maxresdefault.jpg" alt="FURo-Sim Demo Video" width="100%"/>
  </a>
  <br/>
  <em>▶ Demo Video (click to watch on YouTube)</em>
</p>

> **Notice:** This repository is a slimmed-down distribution of FURo-Sim focused on the **ROS 2 car teleoperation pipeline**. The Python client, ROS 1 wrapper, AUV examples, and Unreal Engine project sources are not included here.

FURo-Sim is a GPU-accelerated, physics-based simulation framework built on [Unreal Engine 5.4](https://www.unrealengine.com/), originally derived from [AirSim](https://github.com/microsoft/AirSim) and [Colosseum](https://github.com/CodexLabsLLC/Colosseum). This distribution provides the ROS 2 (Humble) integration for driving the simulated `PhysXCar` via `cmd_vel`.

---

## Key Features

- ROS 2 (Humble) integration with the simulator
- `cmd_vel` (`geometry_msgs/Twist`) → `CarControls` translation with a P-controller for closed-loop velocity tracking
- Map-scale-aware velocity scaling (`cmd_vel_linear_scale` parameter)
- Keyboard teleoperation via `teleop_twist_keyboard`

---

## Supported Environment

| Component | Version |
|---|---|
| OS | Ubuntu 22.04 (native or WSL2) |
| ROS 2 | Humble |
| Simulator | Pre-built FURo-Sim binary (Unreal Engine 5.4) |

---

## Repository Structure

```
FURo-Sim/
├── AirLib/                 # AirSim core simulation library (build dependency)
├── MavLinkCom/             # MavLink communication library (build dependency)
├── cmake/                  # CMake glue for AirLib / MavLinkCom / rpclib
├── external/               # rpclib (RPC) source
├── ros2/                   # ROS 2 (Humble) workspace
│   ├── src/
│   │   ├── furosim_interfaces/    # Custom message/service definitions
│   │   └── furosim_ros_pkgs/      # Wrapper node, launch files
│   └── car_teleop.sh              # Keyboard teleop helper script
├── settings.json           # Example AirSim settings (Car + camera)
├── setup.sh                # Install build prerequisites
├── build.sh                # Build AirLib / MavLinkCom / rpclib
├── clean.sh                # Clean build artifacts
└── LICENSE
```

---

## Build from Source (WSL2 / Ubuntu 22.04)

The simulator binary runs on the Windows host; ROS 2 development happens inside WSL2.

### 1. Install dependencies and build AirLib

From the repository root:

```bash
# One-time: install system packages (clang, cmake, etc.)
./setup.sh

# Build AirLib, MavLinkCom, rpclib
./build.sh
```

### 2. Build the ROS 2 workspace

```bash
cd ros2
source /opt/ros/humble/setup.bash
colcon build --packages-select furosim_interfaces furosim_ros_pkgs
source install/setup.bash
```

After this, the launch files under `furosim_ros_pkgs` are available.

---

## Simulator Settings (`settings.json`)

The simulator reads `settings.json` from `~/Documents/FURo-Sim/settings.json` on Linux, or `Documents\FURo-Sim\settings.json` on Windows. An example tailored for car teleoperation with a front-center RGB camera is provided at the repository root.

### Install the example

**Linux / WSL** (when the simulator is run from inside WSL):
```bash
mkdir -p ~/Documents/FURo-Sim
cp settings.json ~/Documents/FURo-Sim/settings.json
```

**Windows** (when the simulator binary is run from Windows, which is the common case with WSL2):
```powershell
# In PowerShell, replace <user> with your Windows username
mkdir $HOME\Documents\FURo-Sim -Force
copy \\wsl$\Ubuntu-22.04\home\<user>\ta_tack\FURo-Sim\settings.json $HOME\Documents\FURo-Sim\settings.json
```

### What it configures

- `SimMode: Car` — boot the simulator in car mode
- One vehicle named `PhysXCar` using AirSim's built-in PhysX-based car
- One front-center RGB camera (640×480, 64.4° FOV, pitched 15° down)

Restart the simulator after changing this file.

---

## Run

In three separate terminals (all sourced with `install/setup.bash`):

**1. Start the simulator binary** (on the Windows host)

**2. Start the ROS 2 wrapper node**
```bash
ros2 launch furosim_ros_pkgs furosim_node.launch.py
```

Useful parameters (override on the command line, e.g. `cmd_vel_linear_scale:=1.0`):
- `cmd_vel_linear_scale` — multiplier applied to `cmd_vel.linear.x` (default `10.0` for the current 10× scaled map)
- `cmd_vel_angular_scale` — multiplier for `cmd_vel.angular.z` (default `1.0`; angular maps to wheel steering angle which is map-scale independent)
- `host` — IP of the simulator host (default `localhost`)

**3. Start the keyboard teleop**
```bash
ros2 launch furosim_ros_pkgs car_teleop.launch.py
```
This opens an `xterm` window. If `xterm` does not appear under WSL, use the fallback helper script instead:
```bash
./car_teleop.sh                       # PhysXCar, speed=0.3, turn=1.0
./car_teleop.sh PhysXCar 0.5 0.8      # custom vehicle / speed / turn
```

### Key bindings (`teleop_twist_keyboard`)

```
   u    i    o
   j    k    l
   m    ,    .
```

- `i` / `,` — forward / reverse
- `u` / `o` — forward + left / right
- `m` / `.` — reverse + left / right
- `j` / `l` — turn left / right in place
- `k` — stop
- `q` / `z` — increase / decrease both linear and angular by 10%
- `w` / `x` — linear only
- `e` / `c` — angular only

### Verify topics

```bash
ros2 topic list
ros2 topic echo /furosim_node/PhysXCar/cmd_vel
ros2 topic echo /furosim_node/PhysXCar/car_state    # speed, gear, rpm
```

---

## Citation

```bibtex
@inproceedings{kweon2025furosim,
  title={Development of Maritime Robotics Simulator Using Unreal Engine 5},
  author={Kweon, Heekyu and Sim, Hyeonmin and Joe, Hangil},
  booktitle={2025 22nd International Conference on Ubiquitous Robots (UR)},
  year={2025},
  organization={IEEE}
}
```

---

## License

This project is released under the MIT License. FURo-Sim modifications: Copyright (c) FURo Lab.

See [LICENSE](LICENSE) for full details, including the original AirSim (Microsoft) and Colosseum (Codex Laboratories LLC) licenses.

---

## Acknowledgements

- [AirSim](https://github.com/microsoft/AirSim) — Microsoft Corporation
- [Colosseum](https://github.com/CodexLabsLLC/Colosseum) — Codex Laboratories LLC

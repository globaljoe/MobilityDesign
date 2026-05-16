# MobilityDesign

## Build from Source (WSL2 / Ubuntu 22.04)

From the repository root:

```bash
./setup.sh
./build.sh

cd ros2
source /opt/ros/humble/setup.bash
colcon build --packages-select furosim_interfaces furosim_ros_pkgs
source install/setup.bash
```

---

## Simulator Settings (`settings.json`)

The simulator reads `settings.json` from `Documents\FURo-Sim\settings.json` on Windows.

---

## Run

In separate terminals (all sourced with `install/setup.bash`):

```bash
# 1) Start the simulator binary (on the Windows host)

# 2) ROS 2 wrapper node
ros2 launch furosim_ros_pkgs furosim_node.launch.py

# 3) Keyboard teleop
ros2 launch furosim_ros_pkgs car_teleop.launch.py
# or, if xterm does not pop under WSL:
./ros2/car_teleop.sh
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
- `j` / `l` — turn in place
- `k` — stop
- `q` / `z` — scale both linear and angular by ±10%
- `w` / `x` — linear only
- `e` / `c` — angular only

---

## License

Released under the MIT License. See [LICENSE](LICENSE), including the original AirSim (Microsoft) and Colosseum (Codex Laboratories LLC) license notices.

## Acknowledgements

- [AirSim](https://github.com/microsoft/AirSim) — Microsoft Corporation
- [Colosseum](https://github.com/CodexLabsLLC/Colosseum) — Codex Laboratories LLC

#!/usr/bin/env bash
# Keyboard teleop for the FURo-Sim car via cmd_vel.
# Usage:
#   ./car_teleop.sh                          # defaults: vehicle=PhysXCar, speed=0.3, turn=1.0
#   ./car_teleop.sh OtherCar                 # custom vehicle name
#   ./car_teleop.sh PhysXCar 0.5 0.8         # vehicle, speed, turn
set -e

VEHICLE_NAME="${1:-PhysXCar}"
SPEED="${2:-0.3}"
TURN="${3:-1.0}"
SIM_NODE="${SIM_NODE:-furosim_node}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ -f "${SCRIPT_DIR}/install/setup.bash" ]]; then
    # shellcheck disable=SC1091
    source "${SCRIPT_DIR}/install/setup.bash"
fi

CMD_VEL_TOPIC="/${SIM_NODE}/${VEHICLE_NAME}/cmd_vel"
echo "Publishing to: ${CMD_VEL_TOPIC}"
echo "speed=${SPEED}  turn=${TURN}"
exec ros2 run teleop_twist_keyboard teleop_twist_keyboard \
    --ros-args \
    -r "/cmd_vel:=${CMD_VEL_TOPIC}" \
    -p "speed:=${SPEED}" \
    -p "turn:=${TURN}"

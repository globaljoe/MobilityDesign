// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AsvParams_hpp
#define air_AsvParams_hpp

#include "common/Common.hpp"

namespace msr
{
namespace airlib
{

/**
 * AsvVehicleParams
 *
 * Physical and hydrodynamic parameters for a catamaran-type ASV (surface vehicle).
 * Models 3-DOF horizontal plane motion: surge (u), sway (v), yaw (r).
 * In full 6-DOF fluid-flux mode (surface_constrained = false), Ixx/Iyy are also used.
 *
 * All quantities use SI units.
 * Coordinate convention: NED body frame (X forward, Y starboard, Z down).
 * Notation follows SNAME (1950) / Fossen convention.
 *
 * Reference: T. I. Fossen, "Handbook of Marine Craft Hydrodynamics and Motion Control"
 */
struct AsvVehicleParams
{
    // ── simulation mode ──────────────────────────────────────────────────
    // true  → water surface constraint (lock Z translation + X/Y rotation, disable gravity)
    // false → full 6-DOF fluid-flux simulation
    bool surface_constrained = true;

    // ── basic properties ─────────────────────────────────────────────────
    // Source: BlueBoat datasheet v1.1 (Jan 2025), bluerobotics.com
    float mass = 14.5f;  // [kg]     vehicle dry weight (no batteries, no payload)
    float Izz  = 0.80f;  // [kg·m²]  yaw   moment of inertia (m * L² / 12 ≈ 14.5 * 1.2² / 18)
    float Ixx  = 0.20f;  // [kg·m²]  roll  moment of inertia (6-DOF mode only)
    float Iyy  = 0.60f;  // [kg·m²]  pitch moment of inertia (6-DOF mode only)

    // ── geometry ─────────────────────────────────────────────────────────
    // All positions in NED body frame, relative to the mesh/physics origin. [m]
    //
    // center_of_gravity: CoM offset from mesh origin.
    //   BlueBoat: roughly centered; small Z offset due to battery placement.
    Vector3r center_of_gravity = Vector3r(0.0f, 0.0f, 0.02f);  // [m] NED

    // thruster_left_pos / thruster_right_pos:
    //   Position of each thruster's thrust application point relative to CoM. [m] NED
    //   BlueBoat: thrusters mounted ~0.35 m port/starboard of centerline,
    //             roughly at CoM longitudinal station, at waterline depth.
    //   → Mz = r_left × F_left + r_right × F_right  (2-D: Mz = -y_L*F_L - y_R*F_R
    //           but r_L.y = -half_beam, r_R.y = +half_beam, so Mz = (F_R - F_L)*half_beam)
    Vector3r thruster_left_pos  = Vector3r(-0.5f, -0.3f, 0.28f);  // [m] NED  (port)
    Vector3r thruster_right_pos = Vector3r(-0.5f,  0.3f, 0.28f);  // [m] NED  (starboard)

    float max_thrust_N = 50.0f;  // [N]  max forward thrust per M200 (5.1 kgf @ 16V)

    // ── linear damping ───────────────────────────────────────────────────
    // D_lin = diag(Xu, Yv, Nr)
    float Xu = -5.0f;   // [N·s/m]      surge linear damping
    float Yv = -8.0f;   // [N·s/m]      sway  linear damping
    float Nr = -2.0f;   // [N·m·s/rad]  yaw   linear damping

    // ── quadratic damping ────────────────────────────────────────────────
    // D_quad = diag(Xuu|u|, Yvv|v|, Nrr|r|)
    float Xuu = -10.0f;  // [N·s²/m²]      surge quadratic damping
    float Yvv = -15.0f;  // [N·s²/m²]      sway  quadratic damping
    float Nrr =  -5.0f;  // [N·m·s²/rad²]  yaw   quadratic damping

    // ── added mass ───────────────────────────────────────────────────────
    // M_A = diag(-Xdu, -Ydv): used for added mass Coriolis (C_A)
    float Xdu = -2.0f;   // [kg]   surge added mass
    float Ydv = -8.0f;   // [kg]   sway  added mass

    // ── 6-DOF roll/pitch damping (surface_constrained = false only) ─────
    // Restoring moments are handled externally by fluid flux.
    float Kp = -0.30f;  // [N·m·s/rad]  roll  damping
    float Mq = -0.80f;  // [N·m·s/rad]  pitch damping
};

} //namespace airlib
} //namespace msr

#endif
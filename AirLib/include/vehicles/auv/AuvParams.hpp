// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef air_AuvParams_hpp
#define air_AuvParams_hpp

#include "common/Common.hpp"

namespace msr
{
namespace airlib
{

/**
 * AuvVehicleParams
 *
 * Physical and hydrodynamic parameters for an AUV body.
 * All quantities use SI units; vectors are in NED frame.
 *
 * Notation follows SNAME (1950) convention:
 *   u/v/w  : surge / sway / heave  (linear)
 *   p/q/r  : roll  / pitch / yaw   (angular)
 */
struct AuvVehicleParams
{
    // ── basic properties ─────────────────────────────────────────────────
    float mass     = 11.5f;   // [kg]
    float weight   = 112.8f;  // [N]
    float buoyancy = 114.8f;  // [N]

    Vector3r center_of_gravity  = Vector3r(0.f, 0.f, 0.02f); // NED [m]
    Vector3r center_of_buoyancy = Vector3r::Zero();            // NED [m]

    // ── moments of inertia ───────────────────────────────────────────────
    float Ixx = 0.16f, Iyy = 0.16f, Izz = 0.16f; // [kg·m²]

    // ── linear damping ───────────────────────────────────────────────────
    float xu = -4.03f, xv = -6.22f, xw = -5.18f; // [N·s/m]
    float kp = -0.07f, mq = -0.07f, nr = -0.07f;  // [N·m·s/rad]

    // ── quadratic damping ────────────────────────────────────────────────
    float xuu = -18.18f, xvv = -21.66f, xww = -36.99f; // [N·s²/m²]
    float kpp = -1.55f,  mqq = -1.55f,  nrr = -1.55f;  // [N·m·s²/rad²]

    // ── added mass ───────────────────────────────────────────────────────
    float axu = -5.5f,  ayv = -12.7f,  azw = -14.57f; // [kg]
    float akp = -0.12f, amq = -0.12f,  anr = -0.12f;  // [kg·m²]
};

} //namespace airlib
} //namespace msr

#endif

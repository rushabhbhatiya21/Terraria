#pragma once
#include <cmath>
#include <math/vec2.h>

struct SecondOrderDynamics
{
    // state
    Engine::Vec2 xp;   // previous input
    Engine::Vec2 y;    // output
    Engine::Vec2 yd;   // output velocity

    // constants
    float k1, k2, k3;

    //SecondOrderDynamics(float f, float z, float r, Engine::Vec2 x0)
    //{
    //    // compute constants
    //    float pi = 3.14159265359f;
    //    k1 = z / (pi * f);
    //    k2 = 1.0f / ((2.0f * pi * f) * (2.0f * pi * f));
    //    k3 = r * z / (2.0f * pi * f);

    //    // init
    //    xp = x0;
    //    y = x0;
    //    yd = { 0.0f, 0.0f };
    //}

    void init(float f, float z, float r, Engine::Vec2 x0)
    {
        // compute constants
        float pi = 3.14159265359f;
        k1 = z / (pi * f);
        k2 = 1.0f / ((2.0f * pi * f) * (2.0f * pi * f));
        k3 = r * z / (2.0f * pi * f);

        // init
        xp = x0;
        y = x0;
        yd = { 0.0f, 0.0f };
    }

    Engine::Vec2 Update(float dt, Engine::Vec2 x, Engine::Vec2 xd = { 0.0f, 0.0f }, bool hasXd = false)
    {
        if (dt <= 0.0f) return y;

        // estimate velocity if not provided
        if (!hasXd)
        {
            xd.x = (x.x - xp.x) / dt;
            xd.y = (x.y - xp.y) / dt;
            xp = x;
        }

        // stability clamp
        float k2_stable = fmaxf(k2, fmaxf(dt * dt / 2.0f + dt * k1 / 2.0f, dt * k1));

        // integrate position
        y.x += dt * yd.x;
        y.y += dt * yd.y;

        // integrate velocity
        yd.x += dt * (x.x + k3 * xd.x - y.x - k1 * yd.x) / k2_stable;
        yd.y += dt * (x.y + k3 * xd.y - y.y - k1 * yd.y) / k2_stable;

        return y;
    }
};
#pragma once

#include <unordered_map>
#include <cmath>
#include <raylib.h>

// =========================
// Shake data
// =========================
struct ShakeData
{
    float time;
    float duration;
    float strength;
    float phase;
};

// =========================
// Storage
// =========================
static std::unordered_map<long long, ShakeData> shakeMap;

// =========================
// Key generator (x,y → unique key)
// =========================
inline long long getKey(int x, int y)
{
    return (static_cast<long long>(y) << 32) | static_cast<unsigned int>(x);
}

// =========================
// Trigger shake
// =========================
inline void triggerShake(int x, int y)
{
    long long key = getKey(x, y);

    shakeMap[key] = {
        0.15f,   // time
        0.15f,   // duration
        0.1f,   // strength (IMPORTANT: small because your tile size = 1)
        GetRandomValue(0, 1000) / 100.0f // phase
    };
}

// =========================
// Update (call once per frame)
// =========================
inline void updateShake(float dt)
{
    for (auto it = shakeMap.begin(); it != shakeMap.end(); )
    {
        it->second.time -= dt;

        if (it->second.time <= 0.0f)
            it = shakeMap.erase(it);
        else
            ++it;
    }
}

// =========================
// Get offset for a tile
// =========================
inline Vector2 getShakeOffset(int x, int y)
{
    long long key = getKey(x, y);

    auto it = shakeMap.find(key);
    if (it != shakeMap.end())
    {
        const ShakeData& s = it->second;

        float t = s.time / s.duration; // fade 1 → 0
        float strength = s.strength * t;

        float time = (float)GetTime();

        return {
            sinf(time * 40.0f + s.phase) * strength,
            cosf(time * 40.0f + s.phase) * strength
        };
    }

    return { 0.0f, 0.0f };
}

struct CameraShake
{
    float time = 0.0f;
    float duration = 0.0f;
    float strength = 0.0f;
    float phase = 0.0f;

    void triggerCameraShake(float duration, float strength);

    void updateCameraShake(float dt);
};

extern CameraShake camShake;
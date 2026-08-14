#include "shake.h"

std::unordered_map<long long, ShakeData> shakeMap;

CameraShake camShake;

void CameraShake::triggerCameraShake(float duration, float strength)
{
    this->time = duration;
    this->duration = duration;
    this->strength = strength;
    this->phase = randomFloat(0, 1000) / 100.0f;
}

void CameraShake::updateCameraShake(float dt)
{
    if (this->time > 0.0f)
    {
        this->time -= dt;
        if (this->time < 0.0f)
            this->time = 0.0f;
    }
}
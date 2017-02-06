#pragma once

#include "../game.h"

class EnemySpawner
    : public GameComponent
{
    vec2 _pos = {};
    float _spawnTimer = 5;
    Sprite _spr;

public:
    virtual void Draw() override;
    virtual void Update() override;

    EnemySpawner(vec2 pos, Game &);
};
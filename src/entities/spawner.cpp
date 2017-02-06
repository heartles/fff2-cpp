#include "spawner.h"

#include "enemy.h"

const float SpawnInterval = 5;

EnemySpawner::EnemySpawner(vec2 pos, Game &g)
    : GameComponent(g), _pos(pos)
{
    _spr = Engine.Content.LoadSprite(Engine.GameDir + "/content/burgerbox.png");
}

void
EnemySpawner::Update()
{
    _spawnTimer -= Engine.DT;
    if (_spawnTimer <= 0) {
        Engine.AddComponent(new Enemy(_pos, Engine));

        _spawnTimer += SpawnInterval;
    }
}

void
EnemySpawner::Draw()
{
    Engine.View.DrawSprite(_spr, _pos);
}
#include "bullet.h"

Bullet::Bullet(Game& g, const vec2& pos, const vec2& vel, float rot)
  : GameComponent(g)
  , _pos(pos)
  , _vel(vel)
  , _rot(rot)
{
    _spr = g.Content.LoadSprite(g.GameDir + "/content/Bullet_spr_0.png");
}

void
Bullet::Update()
{
    _pos += _vel * Engine.DT;
    Rectangle mask{ _pos.x, _pos.y, 1.0f / 10, 1.0f / 10 };
    for (auto s : Engine.Statics) {
        if (s.Rect.Intersects(mask)) {
            Engine.RemoveComponent(this);
        }
    }
}

void
Bullet::Draw()
{
    glm::mat3 modelView =
      Scale({ 2 / Engine.View.Width(), 2 / Engine.View.Height() }) *
      Translate({ -Engine.View.X, -Engine.View.Y }) *
      Translate({ _pos.x, _pos.y }) * Rotate(-_rot);

    DEBUG_DrawSprite(_spr, modelView, FullImage, Colors::White);
}

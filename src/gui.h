#pragma once

struct Game;
class GUIComponent
{
  public:
    inline virtual void Draw() {}

  protected:
    Game& Engine;
    inline GUIComponent(Game& g)
      : Engine(g)
    {
    }

    inline virtual ~GUIComponent() {}
};

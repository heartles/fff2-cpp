#pragma once

#include <string>

#include <json/json.h>

#include "common.h"
#include "content.h"
#include "graphics.h"
#include "gui.h"
#include "math.h"

typedef Sprite (*PFN_LOADIMAGE)(std::string filename);
typedef void (*PFN_DRAWSPRITE)(Sprite, struct Rectangle, struct Rectangle,
                               float);

struct Tile
{
    int TilesetIndex, TileIndex;
    float X, Y;
};

#pragma pack(push)
struct Tileset
{
    std::string ImageName;
    int TileWidth, TileHeight;
    int TileCountX, TileCountY;
    int PixelCountX, PixelCountY;
    int TileCountTotal, TileStart;
    Sprite Image;

    std::vector<vec2> Texcoords;
    std::vector<vec3> Positions;

    GLuint VertexBufferIDs[2];
    GLuint VertexArrayID;
    std::vector<Tile> Tiles;
};
#pragma pack(pop)

struct Game;
class GameComponent
{
  public:
    inline virtual void Update() {}
    inline virtual void Draw() {}
    inline virtual ~GameComponent() {}
    Game& Engine;

  protected:
    inline GameComponent(Game& g)
      : Engine(g)
    {
    }
};

// Invariant: HalfWidth/HalfHeight are positive
struct BoundingBox
{
    Rectangle Rect;
};

struct Game
{
    std::string GameDir;

    float DT;
    Input OldInput;
    struct Input Input;
    bool ShouldClose;

    vec4 ClearColor;

    OrthoView View;
    OrthoView Screen;

    std::vector<GameComponent*> Components;
    std::vector<GUIComponent*> GUIComponents;
    std::vector<Tileset> Tilesets;
    std::vector<Tile> Tiles;

    ContentManager Content;

    std::vector<BoundingBox> Statics;

    std::vector<GameComponent *> componentAddQueue, componentRmQueue;
    inline void AddComponent(GameComponent* c)
    {
        componentAddQueue.push_back(c);
    }
    inline void RemoveComponent(GameComponent* c)
    {
        componentRmQueue.push_back(c);
    }

    std::vector<GUIComponent *> guiComponentAddQueue, guiComponentRmQueue;
    inline void AddComponent(GUIComponent* c)
    {
        guiComponentAddQueue.push_back(c);
    }
    inline void RemoveComponent(GUIComponent* c)
    {
        guiComponentRmQueue.push_back(c);
    }

    inline bool KeyPressed(int id)
    {
        return Input.Keyboard[id] && !OldInput.Keyboard[id];
    }

    template <typename T>
    inline T* GetInstanceOf()
    {
        for (auto c : Components) {
            T* val = dynamic_cast<T*>(c);
            if (val)
                return val;
        }

        return nullptr;
    }
};

void Game_Init(Game&);
void LoadLevel(const std::string& fileLoc, Game& info);
void ParseTileLayer(Json::Value& layer, Game& info, int roomWidth);
void Game_Update(Game&);
void Game_Render(Game&);
vec2 ToGame(Game& info, vec2 screen);
void ResolveCollision(Rectangle mask, vec2* pos, Game& engine);

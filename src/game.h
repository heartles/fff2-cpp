#pragma once

#include "common.h"
#include "graphics.h"
#include "math.h"
#include <json/json.h>
#include <string>

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
    int TileCountTotal;
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
    virtual void Update();
    virtual void Draw();
    virtual void DrawGUI();
    Game& Engine;

  protected:
    inline GameComponent(Game& g)
      : Engine(g)
    {
    }
};

struct Game
{
    std::string GameDir;

    float DT;
    Input OldInput;
    struct Input Input;
    bool ShouldClose;

    struct Rectangle View;

    std::vector<GameComponent*> Components;
    std::vector<Tileset> Tilesets;
    std::vector<Tile> Tiles;

    std::vector<GameComponent*> tempComponents;
    inline void AddComponent(GameComponent *c)
    {
        tempComponents.push_back(c);
    }
};

void Game_Init(Game&);
void LoadLevel(const std::string& fileLoc, Game& info);
void ParseTileLayer(Json::Value& layer, Game& info, int roomWidth);
void Game_Update(Game&);
void Game_Render(Game&);
vec2 ToGame(Game& info, vec2 screen);

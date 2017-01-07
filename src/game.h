#pragma once

#include <string>
#include <json/json.h>
#include "graphics.h"
#include "common.h"
#include "math.h"
struct Game;
#include "player.h"

typedef Sprite (*PFN_LOADIMAGE)(std::string filename);
typedef void (*PFN_DRAWSPRITE)(Sprite, struct Rectangle, struct Rectangle, float);

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


struct CollisionMask
{

};

struct Game
{
    std::string GameDir;

    float DT;
    Input OldInput;
    struct Input Input;
    bool ShouldClose;

    struct Player Player;

    struct Rectangle View;

    //b2Debug _debug;

    std::vector<Tileset> Tilesets;
    std::vector<Tile> Tiles;
};

void Game_Init(Game&);
void LoadLevel(const std::string &fileLoc, Game & info);
void ParseTileLayer(Json::Value &layer, Game & info, int roomWidth);
void Game_Update(Game&);
void Game_Render(Game&);
vec2 ToGame(Game& info, vec2 screen);

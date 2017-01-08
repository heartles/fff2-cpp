#include "game.h"
#include "GLFW/glfw3.h"
#include "content.h"
#include "glm/common.hpp"
#include "graphics.h"
#include "math.h"
#include "shader.h"
#include "json/json.h"
#include <cmath>
#include <fstream>

#include <iostream>

#include "player.h"

using namespace std;

struct Rectangle
FromPixels(Sprite spr, struct Rectangle r)
{
    struct Rectangle output;
    output.X = r.X / spr.Width;
    output.Width = r.Width / spr.Width;
    output.Y = r.Y / spr.Height;
    output.Height = r.Height / spr.Height;

    return output;
}

vec2
ToGame(Game& info, vec2 screen)
{
    vec2 result;

    result.x = screen.x * info.View.Width / 2 + info.View.X;
    result.y = screen.y * info.View.Height / 2 + info.View.Y;

    return result;
}

void
Game_Init(Game& info)
{
    Log("creating world");
    info.View = { 0, 0, 30, 16.875f };

    StackAlloc alloc(1024 * 1024);
    auto shader = LoadShader(info.GameDir + "/content/textured.gl.vert",
                             info.GameDir + "/content/textured.gl.frag");

    shader.Apply();

    string fileLoc = info.GameDir + "/content/mp_grid.json";

    vector<Tileset>* Tilesets = &info.Tilesets;

    LoadLevel(fileLoc, info);
}

void
LoadLevel(const std::string& fileLoc, Game& info)
{
    Json::Value root;
    fstream file;
    file.open(fileLoc);
    Log("1");
    Json::Value val;
    file >> val;
    Log("2");
    auto tilesetsJson = val["tilesets"];
    info.Tilesets.reserve(tilesetsJson.size());

    auto tileWidth = val["tilewidth"].asInt();
    auto tileHeight = val["tileheight"].asInt();

    int roomWidth = val["width"].asInt();
    int roomHeight = val["height"].asInt();
    std::cout << tileWidth;
    std::cout << tileHeight;

    for (auto tileset : tilesetsJson) {
        Tileset t;

        if (tileset["properties"]["type"] != "tilemap")
            continue;

        auto imageString = tileset["image"].asString();

        t.ImageName = imageString.substr(imageString.find_last_of('/'));
        t.PixelCountY = tileset["imageheight"].asInt();
        t.PixelCountX = tileset["imagewidth"].asInt();
        t.TileWidth = tileset["tilewidth"].asInt();
        t.TileHeight = tileset["tileheight"].asInt();
        t.TileCountX = t.PixelCountX / t.TileWidth;
        t.TileCountY = t.PixelCountY / t.TileHeight;
        t.TileCountTotal = tileset["tilecount"].asInt();

        t.Image = DEBUG_LoadSprite(info.GameDir + "/content" + t.ImageName);

        info.Tilesets.push_back(t);
    }

    auto layersJSON = val["layers"];
    for (auto layer : layersJSON) {
        if (layer["type"] == "tilelayer")
            ParseTileLayer(layer, info, roomWidth);
        else if (layer["type"] == "objectgroup") {
            for (auto obj : layer["objects"]) {
                std::string type = obj["type"].asString();

                if (type == "Player") {
                    auto halfWidth = obj["width"].asFloat() / 2 / 64;

                    auto halfHeight = obj["height"].asFloat() / 2 / 64;

                    auto pos = vec2{ obj["x"].asFloat() / tileWidth,
                                     -obj["y"].asFloat() / tileHeight };
                    std::cout << pos.x << ", " << pos.y << std::endl;

                    info.Components.push_back(new Player(info, pos));
                }
            }
        }
    }

    for (auto& t : info.Tilesets) {
        glGenVertexArrays(1, &t.VertexArrayID);
        GLenum err = glGetError();
        glBindVertexArray(t.VertexArrayID);

        glGenBuffers(2, t.VertexBufferIDs);

        glBindBuffer(GL_ARRAY_BUFFER, t.VertexBufferIDs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vector(t.Positions),
                     t.Positions.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, t.VertexBufferIDs[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof_vector(t.Texcoords),
                     t.Texcoords.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    Log("3");
}

void
ParseTileLayer(Json::Value& layer, Game& info, int roomWidth)
{
    auto data = layer["data"];
    for (int i = 0; i < data.size(); ++i) {
        auto tile = data[i];

        int tileIndex = tile.asInt();

        if (tileIndex == 0)
            continue;

        tileIndex -= 1;

        int tilesetIndex;

        for (int j = 0; j < info.Tilesets.size(); ++j) {
            tileIndex -= info.Tilesets[j].TileCountTotal;
            if (tileIndex < 0) {
                tileIndex += info.Tilesets[j].TileCountTotal;
                tilesetIndex = j;
                break;
            }
        }

        auto& tileset = info.Tilesets[tilesetIndex];

        // Position given from top left corner of tile
        vec2 position{ float(i % roomWidth), float(-i / roomWidth) };

        // tri 1
        tileset.Positions.push_back({
          position.x, position.y - 1, 1.0f,
        }); // bottom left
        tileset.Positions.push_back({
          position.x + 1, position.y - 1, 1.0f,
        }); // bottom right
        tileset.Positions.push_back({
          position.x, position.y, 1.0f,
        }); // top left

        // tri 2
        tileset.Positions.push_back({
          position.x + 1, position.y, 1.0f,
        }); // top right
        tileset.Positions.push_back({
          position.x, position.y, 1.0f,
        }); // top left
        tileset.Positions.push_back({
          position.x + 1, position.y - 1, 1.0f,
        }); // bottom right

        vec2 source{ tileIndex % tileset.TileCountX / (float)tileset.TileCountX,
                     tileIndex / tileset.TileCountX /
                       (float)tileset.TileCountY };
        float dx = 1.0f / tileset.TileCountX;
        float dy = 1.0f / tileset.TileCountY;
        vec2 tl = source;
        vec2 tr{
            source.x + dx, source.y,
        };
        vec2 bl{
            source.x, source.y + dy,
        };
        vec2 br{
            source.x + dx, source.y + dy,
        };

        // tri 1
        tileset.Texcoords.push_back(bl); // bottom left
        tileset.Texcoords.push_back(br); // Bottom right
        tileset.Texcoords.push_back(tl); // top left

        // tri 2
        tileset.Texcoords.push_back(tr); // top right
        tileset.Texcoords.push_back(tl); // top left
        tileset.Texcoords.push_back(br); // Bottom right

        tileset.Tiles.push_back(Tile{ 0, tileIndex, position.x, position.y });
    }
}

void
Game_Update(Game& info)
{
    if (info.Input.Keyboard[GLFW_KEY_ESCAPE])
        info.ShouldClose = true;

    for (auto c : info.Components) {
        c->Update();
    }

    for (auto c : info.tempComponents) {
        info.Components.push_back(c);
    }

    info.tempComponents.clear();
}

void
Game_Render(Game& info)
{
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat3 viewMat = // Identity<mat3>();
      Scale({ 2 / info.View.Width, 2 / info.View.Height }) *
      Translate({ -info.View.X, -info.View.Y });

    for (auto& t : info.Tilesets) {
        glBindVertexArray(t.VertexArrayID);

        glBindTexture(GL_TEXTURE_2D, t.Image.TextureID);

        SetUniform("ModelView", viewMat);

        glEnableVertexAttribArray(0);

        glEnableVertexAttribArray(1);

        glDrawArrays(GL_TRIANGLES, 0, t.Positions.size());
    }

    for (auto c : info.Components) {
        c->Draw();
    }

    for (auto c : info.Components) {
        c->DrawGUI();
    }
}

void
GameComponent::Update()
{
}

void
GameComponent::Draw()
{
}

void
GameComponent::DrawGUI()
{
}

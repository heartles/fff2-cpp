#include "game.h"

#include <cmath>
#include <fstream>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <json/json.h>

#include "content.h"
#include "entities\enemy.h"
#include "graphics.h"
#include "math.h"
#include "player.h"
#include "shader.h"

using namespace std;

struct Rectangle
FromPixels(Sprite spr, struct Rectangle r)
{
    Rectangle output;
    output.X = r.X / spr.Width;
    output.HalfWidth = r.HalfWidth / spr.Width;
    output.Y = r.Y / spr.Height;
    output.HalfHeight = r.HalfHeight / spr.Height;

    return output;
}

vec2
ToGame(Game& info, vec2 screen)
{
    vec2 result;

    result.x = screen.x * info.View.HalfWidth + info.View.X;
    result.y = screen.y * info.View.HalfHeight + info.View.Y;

    return result;
}

void
Game_Init(Game& info)
{
    Log("creating world");
    info.View = { 0, 0, 30 / 2, 16.875f / 2.0f };

    StackAlloc alloc(1024 * 1024);
    auto shader = DEBUG_LoadShader(info.GameDir + "/content/textured.gl.vert",
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

        auto lastSlash = imageString.find_last_of('/');
        t.ImageName = lastSlash != std::string::npos
                        ? imageString.substr(lastSlash)
                        : "/" + imageString;
        t.PixelCountY = tileset["imageheight"].asInt();
        t.PixelCountX = tileset["imagewidth"].asInt();
        t.TileWidth = tileset["tilewidth"].asInt();
        t.TileHeight = tileset["tileheight"].asInt();
        t.TileCountX = t.PixelCountX / t.TileWidth;
        t.TileCountY = t.PixelCountY / t.TileHeight;
        t.TileCountTotal = tileset["tilecount"].asInt();
        t.TileStart = tileset["firstgid"].asInt();

        t.Image =
          info.Content.LoadSprite(info.GameDir + "/content" + t.ImageName);

        info.Tilesets.push_back(t);
    }

    auto layersJSON = val["layers"];
    for (auto layer : layersJSON) {
        if (layer["type"] == "tilelayer")
            ParseTileLayer(layer, info, roomWidth);
        else if (layer["type"] == "objectgroup") {
            for (auto obj : layer["objects"]) {

                auto halfWidth = obj["width"].asFloat() / 2 / 64;

                auto halfHeight = obj["height"].asFloat() / 2 / 64;

                auto pos =
                  vec2{ obj["x"].asFloat() / tileWidth + halfWidth,
                        -obj["y"].asFloat() / tileHeight + halfHeight };

                std::string type = obj["type"].asString();
                if (type == "Player") {
                    info.Components.push_back(new Player(info, pos));
                } else if (type == "InvisWall") {
                    info.Statics.push_back(
                      BoundingBox{ pos.x, pos.y, halfWidth, halfHeight });
                } else if (type == "BasicEnemy") {
                    info.Components.push_back(new Enemy(pos, info));
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

        Tileset* tilesetPtr = nullptr;

        for (auto& t : info.Tilesets) {
            if (tileIndex - t.TileStart >= 0 &&
                tileIndex - t.TileStart - t.TileCountTotal < 0) {
                tilesetPtr = &t;
                tileIndex -= t.TileStart;
            }
        }

        auto& tileset = *tilesetPtr;

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

    for (auto c : info.componentAddQueue) {
        info.Components.push_back(c);
    }
    info.componentAddQueue.clear();

    for (auto c : info.componentRmQueue) {
        for (int i = 0; i < info.Components.size(); i++) {
            if (info.Components[i] == c) {
                info.Components.erase(info.Components.begin() + i);
                break;
            }
        }
    }
    info.componentRmQueue.clear();
}

void
Game_Render(Game& info)
{
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat3 viewMat = // Identity<mat3>();
      Scale({ 2 / info.View.Width(), 2 / info.View.Height() }) *
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

    auto spr =
      info.Content.LoadSprite(info.GameDir + "/content/InvisWall_spr_0.png");
    for (auto s : info.Statics) {
        DEBUG_DrawSprite(spr, viewMat * Translate({ s.Rect.X, s.Rect.Y }) *
                                Scale({ s.Rect.Width(), s.Rect.Height() }),
                         FullImage, 0);
    }
}

void
ResolveCollision(Rectangle mask, vec2* pos, Game& engine)
{
    for (auto s : engine.Statics) {
        if (s.Rect.Intersects(mask)) {
            s.Rect.HalfWidth += mask.HalfWidth;
            s.Rect.HalfHeight += mask.HalfHeight;

            auto angle = atan2f((mask.Y - s.Rect.Y) / s.Rect.HalfHeight,
                                (mask.X - s.Rect.X) / s.Rect.HalfWidth);

            if (angle >= pi / 4 && angle < 3 * pi / 4) // collision from above
                pos->y = s.Rect.Max().y;
            else if (angle >= -3 * pi / 4 &&
                     angle < -pi / 4) // collision from below
                pos->y = s.Rect.Min().y;
            else if (angle >= -pi / 4 && angle < pi / 4) // collision from right
                pos->x = s.Rect.Max().x;
            else // collision from left
                pos->x = s.Rect.Min().x;
        }
    }
}

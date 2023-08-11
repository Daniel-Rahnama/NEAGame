#include "game.hpp"

#include <thread>
#include <vector>
#include <cassert>
#include <bitset>
#include <algorithm>
#include <fstream>
#include <functional>
#include <json/json.h>

#include "entity.hpp"
#include "mob.hpp"

Game::Game(AppData& appdata, Renderer& renderer, Controller& controller, Mixer& mixer)
    : appdata(appdata), renderer(renderer), controller(controller), mixer(mixer) {
    LoadMap();
    
    camera.w = tileMap.GetWidth() * tileMap.GetTileWidth() * 2;
    camera.h = tileMap.GetHeight() * tileMap.GetTileHeight() * 2;
    
    SDL_Texture* t = renderer.CreateTexture(appdata.Resources() + "/sprites/c1.png");

    player = new Player(t, UP, { ((camera.w - 128) / 2), ((camera.h - 128) / 2), 128, 128 }, 1);

    camera.x = player->DSTRect().x - ((appdata.Width() - player->DSTRect().w) / 2);

    if (camera.x < 0) {
        camera.x = 0;
    }

    if (camera.x > camera.w - appdata.Width()) {
        camera.x = camera.w - appdata.Width();
    }

    camera.y = player->DSTRect().y - ((appdata.Height() - player->DSTRect().h) / 2);

    if (camera.y < 0) {
        camera.y = 0;
    }

    if (camera.y > camera.h - appdata.Height()) {
        camera.y = camera.h - appdata.Height();
    }

    mobs.push_back(new Mob(t, UP, { ((camera.w - 128) / 2) + 200, ((camera.h - 128) / 2), 128, 128 }, 1));
}

void Game::Run() {
    if (appdata.Music()) mixer.Play();

    uint8_t TargetFrameDuration = 1000 / appdata.TargetFPS();
    uint8_t FrameDuration;

    uint16_t FrameCount;

    uint32_t TitleTimestamp = SDL_GetTicks();
    uint32_t FrameStart;
    uint32_t FrameEnd;

    bool running = true;

    while (running) {
        FrameStart = SDL_GetTicks();

        controller.HandleInput(running, player);

        UpdateState();

        Update(running, FrameCount);

        renderer.Render(entities, mobs, player, camera);

        FrameEnd = SDL_GetTicks();

        FrameDuration = FrameEnd - FrameStart;

        FrameCount++;

        if (FrameEnd - TitleTimestamp >= 1000) {
            renderer.UpdateWindowTitle(FrameCount);
            FrameCount = 0;
            TitleTimestamp = FrameEnd;
        }

        if (FrameDuration < TargetFrameDuration) {
            SDL_Delay(TargetFrameDuration - FrameDuration);
        }
    }
}

void Game::Update(bool& running, uint16_t& FrameCount) {
    player->UpdateAnimation(FrameCount);
    player->Update(appdata, entities, mobs, camera);

    if (player->state & DEAD) {
        running = false;
    }

    using namespace std::placeholders;
    
    static std::function<bool (const SDL_Rect&, const int&)> EvaluatePlayerCollision = std::bind(Player::EvaluateCollision, player, _1, _2);

    for (Mob*& m : mobs) {
        m->UpdateAnimation(FrameCount);
        m->Update(entities, mobs, EvaluatePlayerCollision, camera);

        if (m->state & DEAD) {
            delete m;
            mobs.erase(std::remove(mobs.begin(), mobs.end(), m), mobs.end());
        }
    }
}

void Game::UpdateState() {
    std::vector<std::future<void>> futures;

    for (Mob*& mob : mobs) {
        futures.push_back(std::async(std::launch::async, [&mob, this]() -> void {
            int x = (mob->DSTRect().x + (mob->DSTRect().w / 2)) - (player->DSTRect().x + (player->DSTRect().w / 2));
            int y = (mob->DSTRect().y + (mob->DSTRect().h / 2)) - (player->DSTRect().y + (player->DSTRect().h / 2));

            mob->state = ((sqrt((x * x) + (y * y)) > 64) ? (mob->state | MOVING) : ((mob->state & ~MOVING) | ((mob->Cooldown() <= 0) ? ATTACKING : 0)));

            if (abs(x) > abs(y)) {
                mob->state = (x > 0) ? (mob->state & ~(0x2) | (0x1)) : (mob->state | (0x3));
            } else {
                mob->state = (y > 0) ? (mob->state & ~(0x3)) : (mob->state & ~(0x1) | (0x2));
            }
        }));
    }

    for (std::future<void>& future : futures) future.wait();
}

void Game::LoadMap() {
    tileMap.ParseFile(appdata.Resources() + "/tilemaps/map.tmx");

    if (tileMap.HasError()) {
        throw (tileMap.GetErrorCode() + ": " + tileMap.GetErrorText()).c_str();
    }

    for (Tmx::Tileset* tileset : tileMap.GetTilesets()) {
        tilesets[tileset->GetName()] = {tileset, renderer.CreateTexture(appdata.Resources() + tileset->GetImage()->GetSource())};
    }

    for (int l = 0; l < tileMap.GetNumTileLayers(); l++) {
        entities.emplace_back(std::vector<Entity*>());
        for (int x = 0; x < tileMap.GetWidth(); x++) {
            for (int y = 0; y < tileMap.GetHeight(); y++) {
                if (tileMap.GetTileLayer(l)->GetTileId(x, y) == 0) continue;
                Tmx::Tileset*& tileset = tilesets[tileMap.GetTileset(tileMap.GetTileLayer(l)->GetTile(x, y).tilesetId)->GetName()].tileset;
                int tilesPerRow = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
                int rows = (tileMap.GetTileLayer(l)->GetTileId(x, y)) / tilesPerRow;
                SDL_Rect srcrect = { (int)((tileMap.GetTileLayer(l)->GetTileId(x, y) - (rows * tilesPerRow)) * tileset->GetTileWidth()), rows * tileset->GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight() };
                SDL_Rect dstrect = { x * tileset->GetTileWidth() * 2, y * tileset->GetTileHeight() * 2, tileset->GetTileWidth() * 2, tileset->GetTileHeight() * 2 };
                entities[l].emplace_back(new Entity(tilesets[tileset->GetName()].texture, srcrect, dstrect));
            }
        }
    }
}

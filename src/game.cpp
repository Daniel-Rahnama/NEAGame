#include "game.hpp"

#include <thread>
#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>

#include "entity.hpp"
#include "mob.hpp"

Game::Game(const std::unique_ptr<AppData>& appdata, const std::unique_ptr<Renderer>& renderer, const std::unique_ptr<Controller>& controller, const std::unique_ptr<Mixer>& mixer)
    : appdata(appdata), renderer(renderer), controller(controller), mixer(mixer) {
    LoadMap();
    
    camera.w = tileMap.GetWidth() * tileMap.GetTileWidth() * 2;
    camera.h = tileMap.GetHeight() * tileMap.GetTileHeight() * 2;
    camera.x = (camera.w - appdata->Width()) / 2;
    camera.y = (camera.h - appdata->Height()) / 2;
}

void Game::Run() {
    if (appdata->Music()) mixer->Play();
    
    SDL_Texture* t = renderer->CreateTexture(appdata->Resources() + "/sprites/c1.png");

    Player e(t, UP, { (camera.w - 128) / 2 , (camera.h - 128) / 2 , 128, 128 }, 0);

    player = &e;

    Player e2(t, UP | MOVING, { ((camera.w - 128) / 2) + 200, ((camera.h - 128) / 2) + 200, 128, 128 }, 0);

    mobs.push_back(&e2);

    uint8_t TargetFrameDuration = 1000 / appdata->TargetFPS();
    uint8_t FrameDuration;

    uint16_t FrameCount;

    uint32_t TitleTimestamp = SDL_GetTicks();
    uint32_t FrameStart;
    uint32_t FrameEnd;

    bool running = true;

    while (running) {
        FrameStart = SDL_GetTicks();

        controller->HandleInput(running, e.state);

        // std::bitset<sizeof(e.state) * 8> state(e.state);

        // std::cout << state << std::endl;

        Update(running, FrameCount, e);

        std::cout << e.Health() << std::endl;

        renderer->Render(entities, mobs, player, camera);

        FrameEnd = SDL_GetTicks();

        FrameDuration = FrameEnd - FrameStart;

        FrameCount++;

        if (FrameEnd - TitleTimestamp >= 1000) {
            renderer->UpdateWindowTitle(FrameCount);
            FrameCount = 0;
            TitleTimestamp = FrameEnd;
        }

        if (FrameDuration < TargetFrameDuration) {
            SDL_Delay(TargetFrameDuration - FrameDuration);
        }
    }
}

void Game::Update(bool& running, uint16_t& FrameCount, Player& player) {
    player.Update(appdata, entities, camera);
    if (!(FrameCount % 5)) player.UpdateAnimation(appdata);

    if (player.Health() <= 0) {
        running = false;
    }

    for (Mob*& m : mobs) {
        m->Update(appdata, entities, camera);
        if (!(FrameCount % 5)) m->UpdateAnimation(appdata);

        if (m->Health() <= 0) {
            delete m;
            mobs.erase(std::remove(mobs.begin(), mobs.end(), m), mobs.end());
        }
    }
}

void Game::LoadMap() {
    tileMap.ParseFile(appdata->Resources() + "/tilemaps/map.tmx");

    if (tileMap.HasError()) {
        throw (tileMap.GetErrorCode() + ": " + tileMap.GetErrorText()).c_str();
    }

    for (Tmx::Tileset* tileset : tileMap.GetTilesets()) {
        tilesets[tileset->GetName()] = {tileset, renderer->CreateTexture(appdata->Resources() + tileset->GetImage()->GetSource())};
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

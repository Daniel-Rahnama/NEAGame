#include "game.hpp"

#include <thread>
#include <iostream>
#include <vector>
#include <cassert>

#include "entity.hpp"
#include "mob.hpp"

Game::Game(const std::shared_ptr<AppData>& appdata, const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Controller>& controller, const std::shared_ptr<Mixer>& mixer)
    : appdata(appdata), renderer(renderer), controller(controller), mixer(mixer) {
    ReadTileMap();
}

void Game::Run() {
    if (appdata->Music()) mixer->Play();
    
    SDL_Texture* t = renderer->CreateTexture(appdata->Resources() + "/sprites/c1.png");

    Mob e(t, {64, 640, 64, 64}, {336, 336, 128, 128}, 0);

    mobs.push_back(&e);

    Uint32 Target_Frame_Duration = 1000 / appdata->TargetFPS();

    Uint32 Title_Timestamp = SDL_GetTicks();
    Uint32 Frame_Start;
    Uint32 Frame_End;
    Uint32 Frame_Duration;

    unsigned int Frame_Count;

    bool running = true;

    while (running) {
        Frame_Start = SDL_GetTicks();

        controller->HandleInput(running, e.direction);

        Update(running, Frame_Count, e);

        renderer->Render(entities, mobs);

        Frame_End = SDL_GetTicks();

        Frame_Duration = Frame_End - Frame_Start;

        Frame_Count++;

        if (Frame_End - Title_Timestamp >= 1000) {
            renderer->UpdateWindowTitle(Frame_Count);
            Frame_Count = 0;
            Title_Timestamp = Frame_End;
        }

        if (Frame_Duration < Target_Frame_Duration) {
            SDL_Delay(Target_Frame_Duration - Frame_Duration);
        }
    }
}

void Game::Update(bool& running, unsigned int& Frame_Count, Mob& entity) {
    entity.Update(appdata, entities);
    if (!(Frame_Count % 5)) entity.UpdateAnimation(appdata);
}

void Game::ReadTileMap() {
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
                SDL_Rect dstrect = { x * tileset->GetTileWidth() * 2, y * tileset->GetTileHeight() * 2, tileset->GetTileWidth() * 2, tileset->GetTileHeight() * 2};
                entities[l].emplace_back(new Entity(tilesets[tileset->GetName()].texture, srcrect, dstrect));
            }
        }
    }
}

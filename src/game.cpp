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
    
    SDL_Texture* t = renderer.CreateTexture(appdata.PlayerSpritesheet());

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

    for (Mob*& m : mobs) {
        m->route = GeneratePath(m);
        m->framesSinceRouteUpdate = rand() % 60;
    }
}

Game::~Game() {
    for (std::vector<Entity*>& vector : entities) for (Entity*& entity : vector) delete entity;
    for (Mob*& mob : mobs) delete mob;
    delete player;
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

// needs to handle at target node, no route, and also when creating a mob the route must be set and then a random framessincerouteupdate set to ensure that it doesnt update at the same time as all other mobs

void Game::UpdateState() {
    std::vector<std::future<void>> futures;

    for (Mob*& mob : mobs) {
        futures.emplace_back(std::async(std::launch::async, [&mob, this]() -> void {
            if (mob->framesSinceRouteUpdate > 60) {
                mob->route = GeneratePath(mob);
                mob->framesSinceRouteUpdate = 0;
            } else {
                mob->framesSinceRouteUpdate++;
            }

            SDL_Point currentNode(mob->DSTRect().x / 64, mob->DSTRect().y / 64);
            SDL_Point nextNode;
            SDL_Point targetNode(player->DSTRect().x / 64, player->DSTRect().y / 64);

            for (int i = 0; i < mob->route.size(); i++) {
                if ((currentNode.x == mob->route[i].x) && (currentNode.y == mob->route[i].y)) {
                    if ((mob->route[i].x == targetNode.x) && (mob->route[i].y == targetNode.y)) {
                        int x = (mob->DSTRect().x + (mob->DSTRect().w / 2)) - (player->DSTRect().x + (player->DSTRect().w / 2));
                        int y = (mob->DSTRect().y + (mob->DSTRect().h / 2)) - (player->DSTRect().y + (player->DSTRect().h / 2));

                        mob->state = ((sqrt((x * x) + (y * y)) > 128) ? (mob->state | MOVING) : ((mob->state & ~MOVING) | ((mob->Cooldown() <= 0) ? ATTACKING : 0)));
                        break;
                    }
                    
                    nextNode = mob->route[i + 1];
                    break;
                }
            }

            if (nextNode.x - currentNode.x == -1) (mob->state & ~(0x2) | (0x1));
            else if (nextNode.x - currentNode.x == 1) ((mob->state | (0x3)));
            else if (nextNode.y - currentNode.y == -1) (mob->state & ~(0x3));
            else if (nextNode.y - currentNode.y == 1) (mob->state & ~(0x1) | (0x2));

        }));
    }

    for (std::future<void>& future : futures) future.wait();
}

std::vector<SDL_Point> Game::GeneratePath(Mob*& mob) {
    std::vector<std::vector<unsigned int>> score;
    std::vector<std::vector<unsigned int>> hScore;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<SDL_Point>> routeToNode;

    for (int x = 0; x < tileMap.GetWidth(); x++) {
        for (int y = 0; y < tileMap.GetHeight(); y++) {
            score[x][y] = -1;
            hScore[x][y] = -1;
            visited[x][y] = false;
        }
    }

    SDL_Point startNode(mob->DSTRect().w / 64, mob->DSTRect().h / 64);
    SDL_Point targetNode(player->DSTRect().w / 64, player->DSTRect().h / 64);

    score[startNode.x][startNode.y] = 0;
    hScore[startNode.x][startNode.y] = 0;

    while (true) {
        SDL_Point currentNode = LowestScoreNode(hScore, visited);
        visited[currentNode.x][currentNode.y] = true;

        std::vector<SDL_Point> neighbours;
        neighbours.emplace_back(SDL_Point(currentNode.x - 1, currentNode.y));
        neighbours.emplace_back(SDL_Point(currentNode.x + 1, currentNode.y));
        neighbours.emplace_back(SDL_Point(currentNode.x, currentNode.y - 1));
        neighbours.emplace_back(SDL_Point(currentNode.x, currentNode.y + 1));

        for (SDL_Point nextNode : neighbours) {
            if (!visited[nextNode.x][nextNode.y] && !tileMap.GetTileLayer(mob->Layer())->GetTileId(nextNode.x, nextNode.y)) {
                int newScore = score[currentNode.x][currentNode.y] + 1;
                if (newScore < score[nextNode.x][nextNode.y]) {
                    score[nextNode.x][nextNode.y] = newScore;
                    hScore[nextNode.x][nextNode.y] = newScore + (abs(nextNode.x - targetNode.x) + abs(nextNode.y - targetNode.y));
                    routeToNode[nextNode.x][nextNode.y] = currentNode;
                }
            }
        }

        if ((currentNode.x == targetNode.x) && (targetNode.y == targetNode.y)) {
            std::vector<SDL_Point> route;
            currentNode = targetNode;

            while (true) {
                route.emplace(route.begin(), routeToNode[currentNode.x][currentNode.y]);

                if ((currentNode.x == startNode.x) && (currentNode.y == startNode.y)) {
                    return route;
                } else {
                    currentNode = routeToNode[currentNode.x][currentNode.y];
                }
            }
        }

        SDL_Point lowestScoreNode = LowestScoreNode(hScore, visited);
        if (score[lowestScoreNode.x][lowestScoreNode.y] == (unsigned int)(-1)) {
            return std::vector<SDL_Point>();
        }
    }
}

SDL_Point Game::LowestScoreNode(const std::vector<std::vector<unsigned int>>& hScore, const std::vector<std::vector<bool>>& visited) {
    SDL_Point result;
    
    for (int x = 0; x < tileMap.GetWidth(); x++) {
        for (int y = 0; y < tileMap.GetHeight(); y++) {
            if (!visited[x][y] && (hScore[x][y] < hScore[result.x][result.y])) {
                result = SDL_Point(x, y);
            }
        }
    }
    
    return result;
}

void Game::LoadMap() {
    tileMap.ParseFile(appdata.Resources() + "/tilemaps/map.tmx");

    if (tileMap.HasError()) {
        throw (tileMap.GetErrorCode() + ": " + tileMap.GetErrorText()).c_str();
    }

    for (Tmx::Tileset* tileset : tileMap.GetTilesets()) {
        tilesets[tileset->GetName()] = { tileset, renderer.CreateTexture(appdata.Resources() + tileset->GetImage()->GetSource()) };
    }

    for (int layer = 0; layer < tileMap.GetNumTileLayers(); layer++) {
        entities.emplace_back(std::vector<Entity*>());
        for (int x = 0; x < tileMap.GetWidth(); x++) {
            for (int y = 0; y < tileMap.GetHeight(); y++) {
                if (tileMap.GetTileLayer(layer)->GetTileId(x, y) == 0) continue;
                Tmx::Tileset*& tileset = tilesets[tileMap.GetTileset(tileMap.GetTileLayer(layer)->GetTile(x, y).tilesetId)->GetName()].tileset;
                int tilesPerRow = tileset->GetImage()->GetWidth() / tileset->GetTileWidth();
                int rows = (tileMap.GetTileLayer(layer)->GetTileId(x, y)) / tilesPerRow;
                SDL_Rect srcrect = { (int)((tileMap.GetTileLayer(layer)->GetTileId(x, y) - (rows * tilesPerRow)) * tileset->GetTileWidth()), rows * tileset->GetTileHeight(), tileset->GetTileWidth(), tileset->GetTileHeight() };
                SDL_Rect dstrect = { x * tileset->GetTileWidth() * 2, y * tileset->GetTileHeight() * 2, tileset->GetTileWidth() * 2, tileset->GetTileHeight() * 2 };
                entities[layer].emplace_back(new Entity(tilesets[tileset->GetName()].texture, srcrect, dstrect, layer));
            }
        }
    }
}

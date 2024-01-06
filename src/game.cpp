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
    LoadMission();
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
    bool won = false;

    uint64_t GameStart = SDL_GetTicks64();

    while (running) {
        FrameStart = SDL_GetTicks();

        controller.HandleInput(running, player);

        UpdateState();

        Update(running, won, FrameCount);

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

    if (won && appdata.DifficultyLevel() != "") {
        uint64_t GameDuration = (SDL_GetTicks64() - GameStart) / 1000;

        if (localLeaderboard != "") {
            int time;

            std::fstream localLeaderboardFile(localLeaderboard, std::fstream::in);
            Json::Value localLeaderboardJson;


            if (localLeaderboardFile.is_open()) {
                localLeaderboardFile >> localLeaderboardJson;
                localLeaderboardFile.close();
            } else {
                localLeaderboardFile.close();
                localLeaderboardJson["time1"] = -1;
                localLeaderboardJson["time2"] = -1;
                localLeaderboardJson["time3"] = -1;
            }

            if (GameDuration < localLeaderboardJson["time" + appdata.DifficultyLevel()].asInt()) {
                localLeaderboardJson["time" + appdata.DifficultyLevel()] = GameDuration;
            }
            
            localLeaderboardFile.open(localLeaderboard, std::fstream::out | std::fstream::trunc);
            localLeaderboardFile << localLeaderboardJson;
            localLeaderboardFile.close();
        }
    }
}

void Game::Update(bool& running, bool& won, uint16_t& FrameCount) {
    player->UpdateAnimation(FrameCount);
    player->Update(appdata, entities, mobs, camera);

    if (player->state & DEAD) {
        running = false;
        won = false;
    }

    using namespace std::placeholders;
    
    static std::function<bool (const SDL_Rect&, const int&)> EvaluatePlayerCollision = std::bind(Player::EvaluateCollision, player, _1, _2);

    for (Mob*& m : mobs) {
        m->UpdateAnimation(FrameCount);
        m->Update(entities, mobs, EvaluatePlayerCollision, camera);

        if (m->state & DEAD) {
            delete m;
            mobs.erase(std::remove(mobs.begin(), mobs.end(), m), mobs.end());
            if (mobs.size() == 0) {
                running = false;
                won = true;
            }
        }
    }
}

void Game::UpdateState() {
    std::vector<std::future<void>> futures;

    for (Mob*& mob : mobs) {
        futures.push_back(std::async(std::launch::async, [&mob, this]() -> void {
            if (mob->framesSinceRouteUpdate > 10) {
                mob->route = GeneratePath(mob);
                mob->framesSinceRouteUpdate = 0;
            } else {
                mob->framesSinceRouteUpdate++;
            }

            SDL_Point currentNode(mob->DSTRect().x / 64, mob->DSTRect().y / 64);
            SDL_Point targetNode(player->DSTRect().x / 64, player->DSTRect().y / 64);

            SDL_Point nextNode(0, 0);

            bool nextNodeFound = false;

            for (int i = 0; i < mob->route.size(); i++) {
                if (currentNode.x == mob->route[i].x && currentNode.y == mob->route[i].y) {
                    nextNode.x = mob->route[i+1].x;
                    nextNode.y = mob->route[i+1].y;
                    nextNodeFound = true;
                }
            }

            if ((abs(currentNode.x - targetNode.x) <= 3 && abs(currentNode.y - targetNode.y) <= 3)
                || (nextNodeFound == false)) {
                int dX = (mob->DSTRect().x + (mob->DSTRect().w / 2)) - (player->DSTRect().x + (player->DSTRect().w / 2));
                int dY = (mob->DSTRect().y + (mob->DSTRect().h / 2)) - (player->DSTRect().y + (player->DSTRect().h / 2));

                if (abs(dX) >= abs(dY)) { 
                    if (dX > 0) mob->state = (mob->state & ~(0x2) | (0x1));
                    else mob->state = (mob->state | (0x3));
                } else {
                    if (dY > 0) mob->state = (mob->state & ~(0x3));
                    else mob->state = (mob->state & ~(0x1) | (0x2));
                }

                if (abs(dX) <= 64 && abs(dY) <= 64) {
                    mob->state |= (mob->Cooldown() <= 0 && mob->Stamina() >= 20) ? ATTACKING : 0;
                }

                if (abs(dX) <= 32 && abs(dY) <= 32) {
                    mob->state &= ~MOVING;
                } else {
                    mob->state |= MOVING;
                }

            } else {
                if (currentNode.x - nextNode.x == 1) mob->state = (mob->state & ~(0x2) | (0x1)); //LEFT
                if (currentNode.x - nextNode.x == -1) mob->state = (mob->state | (0x3)); //RIGHT
                if (currentNode.y - nextNode.y == 1) mob->state = (mob->state & ~(0x3)); //UP
                if (currentNode.y - nextNode.y == -1) mob->state = (mob->state & ~(0x1) | (0x2)); //DOWN

                mob->state |= MOVING;    
            }
        }));

        for (std::future<void>& future : futures) future.wait();
    }
}

std::vector<SDL_Point> Game::GeneratePath(Mob*& mob) {
    std::vector<std::vector<unsigned int>> score(tileMap.GetWidth(), std::vector<unsigned int>(tileMap.GetHeight()));
    std::vector<std::vector<unsigned int>> hScore(tileMap.GetWidth(), std::vector<unsigned int>(tileMap.GetHeight()));
    std::vector<std::vector<bool>> visited(tileMap.GetWidth(), std::vector<bool>(tileMap.GetHeight()));
    std::vector<std::vector<SDL_Point>> routeToNode(tileMap.GetWidth(), std::vector<SDL_Point>(tileMap.GetHeight()));

    for (int x = 0; x < tileMap.GetWidth(); x++) {
        for (int y = 0; y < tileMap.GetHeight(); y++) {
            score[x][y] = -1;
            hScore[x][y] = -1;
            visited[x][y] = false;
        }
    }

    SDL_Point startNode(mob->DSTRect().x / 64, mob->DSTRect().y / 64);
    SDL_Point targetNode(player->DSTRect().x / 64, player->DSTRect().y / 64);

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
                    routeToNode[nextNode.x][nextNode.y] = SDL_Point(currentNode);
                }
            }
        }

        if ((currentNode.x == targetNode.x) && (targetNode.y == targetNode.y)) {
            std::vector<SDL_Point> route;
            currentNode = targetNode;

            for (int i = 0; ; i++) {
                route.emplace(route.begin(), routeToNode[currentNode.x][currentNode.y]);

                if (i == 16 || ((currentNode.x == startNode.x) && (currentNode.y == startNode.y))) {
                    route.erase(route.begin());
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
    unsigned int resultHScore = -1;
    
    for (int x = 0; x < tileMap.GetWidth(); x++) {
        for (int y = 0; y < tileMap.GetHeight(); y++) {
            if (!visited[x][y] && (hScore[x][y] <= resultHScore)) {
                result = SDL_Point(x, y);
                resultHScore = hScore[x][y];
            }
        }
    }
    
    return result;
}

void Game::LoadMission() {
    std::fstream missionFile(appdata.GameMissionAddress(), std::fstream::in);

    if (!missionFile.is_open()) {
        throw "Unable to open Mission file";
    }

    Json::Value missionJson;
    missionFile >> missionJson;
    missionFile.close();

    LoadMap(missionJson["Map Address"].asString());

    SDL_Texture* playerTexture = renderer.CreateTexture(appdata.PlayerSpritesheet());
    SDL_Texture* mobTexture = renderer.CreateTexture(appdata.MobSpritesheet());
        
    camera.w = tileMap.GetWidth() * tileMap.GetTileWidth() * 2;
    camera.h = tileMap.GetHeight() * tileMap.GetTileHeight() * 2;

    player = new Player(playerTexture, (int)NULL, { missionJson["Player"]["x"].asInt() * 64, missionJson["Player"]["y"].asInt() * 64, 128, 128 }, 1, appdata.PlayerStats());

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

    for (Json::Value& m : missionJson["Mobs"]) {
        mobs.push_back(new Mob(mobTexture, (int)NULL, { m["x"].asInt() * 64, m["y"].asInt() * 64, 128, 128 }, 1, appdata.MobStats()));
        mobs.back()->route = GeneratePath(mobs.back());
        mobs.back()->framesSinceRouteUpdate = rand() % 10;
    }

    localLeaderboard = missionJson["Local Leaderboard Address"].asString();
}

void Game::LoadMap(const std::string& mapAddress) {
    tileMap.ParseFile(mapAddress);

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

#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "Tmx.h"
#include "player.hpp"
#include "renderer.hpp"
#include "controller.hpp"
#include "mixer.hpp"
#include "appdata.hpp"
#include "mob.hpp"

#include <string>
#include <memory>
#include <vector>
#include <map>

/// @brief Creates and runs the game
class Game {
public:
    /// @brief Sets the appdata, renderer, controller, and mixer of the game, loads the map and entities
    /// @param appdata Reference to the appdata
    /// @param renderer Reference to the renderer
    /// @param controller Reference to the controller
    /// @param mixer Reference to the mixer
    Game(AppData& appdata, Renderer& renderer, Controller& controller, Mixer& mixer);

    /// @brief Game Deconstructor
    ~Game();
    
    /// @brief Runs the game
    void Run();

    /// @brief Updates the game state every frame
    /// @param running Reference to the running variable in game loop
    /// @param FrameCount Frame count reference
    void Update(bool& running, bool& won, uint16_t& FrameCount);
private:
    /// @brief Changes the Mob's state
    void UpdateState();

    /// @brief Generates a path from the mob to the player using the A* pathfinding algorithm
    /// @param mob Mob from which the path to the player should be generated
    /// @return Path from the mob to the player
    std::vector<SDL_Point> GeneratePath(Mob*& mob);

    /// @brief Finds the node with lowest score for the A* pathfinding algorithm
    SDL_Point LowestScoreNode(const std::vector<std::vector<unsigned int>>& hScore, const std::vector<std::vector<bool>>& visited);

    /// @brief Loads the game mission
    void LoadMission();

    /// @brief Loads the game map
    void LoadMap(const std::string& mapAddress);

    /// @brief x, y - Camera Position, w, h - Game Map Size
    SDL_Rect camera;

    /// @brief TMX Tilemap of game world
    Tmx::Map tileMap;

    /// @brief Contains the tileset and corresponding texture
    struct Tileset {
        /// @brief TMX Tileset of Tile
        Tmx::Tileset* tileset;

        /// @brief Spritesheet of Tile
        SDL_Texture* texture;
    };

    /// @brief Dictionary of tilesets of path of tileset and tileset
    std::map<std::string, Tileset> tilesets;

    /// @brief Game Map
    std::vector<std::vector<Entity*>> entities;

    /// @brief Game Mobs
    std::vector<Mob*> mobs;

    /// @brief Player
    Player* player;

    std::string localLeaderboard;

    /// @brief Reference to the appdata
    AppData& appdata;

    /// @brief Reference to the renderer
    Renderer& renderer;

    /// @brief Reference to the controller
    Controller& controller;

    /// @brief Reference to the mixer
    Mixer& mixer;
};

#endif /* GAME_HPP */
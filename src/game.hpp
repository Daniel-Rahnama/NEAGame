#ifndef GAME_HPP
#define GAME_HPP

#include "SDL2/SDL.h"
#include "Tmx.h"

#include "renderer.hpp"
#include "controller.hpp"
#include "mixer.hpp"
#include "appdata.hpp"
#include "mob.hpp"
#include "player.hpp"

#include <string>
#include <memory>
#include <vector>
#include <map>

/// @brief Creates and runs the game
class Game {
public:
    /// @brief Sets the appdata, renderer, controller, and mixer of the game, loads the map and entities
    /// @param appdata Shared pointer to the appdata
    /// @param renderer Shared pointer to the renderer
    /// @param controller Shared pointer to the controller
    /// @param mixer Shared pointer to the mixer
    Game(const std::shared_ptr<AppData>& appdata, const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Controller>& controller, const std::shared_ptr<Mixer>& mixer);
    
    /// @brief Runs the game
    void Run();

    /// @brief Updates the game state every frame
    /// @param running Reference to the running variable in game loop
    /// @param Frame_Count Frame count reference
    /// @param player Reference to the player
    void Update(bool& running, unsigned int& Frame_Count, Player& player);
private:
    /// @brief Loads the game map
    void LoadMap();

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

    /// @brief Shared pointer to the appdata
    const std::shared_ptr<AppData>& appdata;

    /// @brief Shared pointer to the renderer
    const std::shared_ptr<Renderer>& renderer;

    /// @brief Shared pointer to the controller
    const std::shared_ptr<Controller>& controller;

    /// @brief Shared pointer to the mixer
    const std::shared_ptr<Mixer>& mixer;
};

#endif /* GAME_HPP */
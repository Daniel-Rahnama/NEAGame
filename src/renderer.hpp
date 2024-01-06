#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "appdata.hpp"
#include "entity.hpp"
#include "mob.hpp"
#include "player.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <string>
#include <memory>
#include <vector>

/// @brief Renders the game
class Renderer {
public:
    /// @brief Creates the window and renderer
    /// @param appdata Reference to AppData
    Renderer(AppData& appdata);

    /// @brief Destroys the window and renderer
    ~Renderer();

    /// @brief Renders the game to the window
    /// @param entities Game map
    /// @param mobs Game mobs
    /// @param player Player
    /// @param camera Rectangle representing the camera (x, y - Camera Position, w, h - Game Map Size)
    void Render(std::vector<std::vector<Entity*>>& entities ,std::vector<Mob*>& mobs, Player*& player, const SDL_Rect& camera);
    
    /// @brief Updates the window title with the FPS
    /// @param FPS Frames per second
    void UpdateWindowTitle(const int& FPS);

    /// @brief Creates a SDL texture
    /// @param spritesheet Path to the spritesheet
    /// @return Pointer to the SDL texture
    SDL_Texture* CreateTexture(const std::string& spritesheet);
private:
    /// @brief Merge sort to sort Mobs based on y-coordinate
    /// @param mobs Array of mobs to be sorted
    /// @return Sorted array of mobs
    std::vector<Mob*> static SortMobs(const std::vector<Mob*>& mobs);

    /// @brief Merges mobs for merge sort algorithm
    /// @param left Left array of Mobs
    /// @param right Right array of Mobs
    /// @return Merged array of mobs
    std::vector<Mob*> static MergeMobs(const std::vector<Mob*>& left, const std::vector<Mob*>& right);

    /// @brief SDL Window
    SDL_Window *window;

    /// @brief SDL Renderer
    SDL_Renderer *renderer;

    /// @brief Reference to AppData
    AppData& appdata;
};

#endif /* RENDERER_HPP */
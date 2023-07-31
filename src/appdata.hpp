#ifndef APPDATA_HPP
#define APPDATA_HPP

#include <fstream>
#include <sstream>
#include <mutex>
#include <future>
#include <vector>
#include <json/json.h>

/// @brief Contains the settings for the application
class AppData {
public:
    /// @brief Constructor, reads from the settings file and sets the values of fullscreen, music, width, height, targetFPS, and resources
    AppData();

    /// @brief Path to NEAGame getter
    /// @return Path to NEAGame
    const std::string& Path() const;

    /// @brief Fullscreen Getter
    /// @return 1 if fullscreen is true, 0 if false
    const bool& Fullscreen() const;

    /// @brief Music Getter
    /// @return 1 if music is true, 0 if false
    const bool& Music() const;

    /// @brief Width Getter
    /// @return Width of the window
    const uint16_t& Width() const;

    /// @brief Height Getter
    /// @return Height of the window
    const uint16_t& Height() const;

    /// @brief Target Frames Per Second Getter
    /// @return Target Frames Per Second
    const uint8_t& TargetFPS() const;

    /// @brief Resources Getter
    /// @return Path to the resources folder
    const std::string& Resources() const;
private:
    /// @brief Fullscreen value, 1 if true, 0 if false
    bool fullscreen;

    /// @brief Music value, 1 if true, 0 if false
    bool music;

    /// @brief Width of the window
    uint16_t width;

    /// @brief Height of the window
    uint16_t height;

    /// @brief Target Frames Per Second
    uint8_t targetFPS;

    /// @brief Path to the resources folder
    std::string resources;

    /// @brief Path to the appdata folder
    std::string appdata;

    /// @brief Raw Settings file
    std::fstream settingsFile;

    /// @brief JSON Settings object
    Json::Value settingsJSON;
};

#endif /* APPDATA_HPP */
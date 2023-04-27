#ifndef APPDATA_HPP
#define APPDATA_HPP

#include <fstream>
#include <sstream>
#include <mutex>
#include <future>
#include <vector>

/// @brief Contains the settings for the application
class AppData {
public:
    /// @brief Constructor, reads from the settings file and sets the values of fullscreen, music, width, height, targetFPS, and resources
    AppData();

    /// @brief Destructor, writes current values to the settings file
    ~AppData();

    /// @brief Fullscreen Getter
    /// @return 1 if fullscreen is true, 0 if false
    const bool& Fullscreen();

    /// @brief Music Getter
    /// @return 1 if music is true, 0 if false
    const bool& Music();

    /// @brief Width Getter
    /// @return Width of the window
    const unsigned int& Width();

    /// @brief Height Getter
    /// @return Height of the window
    const unsigned int& Height();

    /// @brief Target Frames Per Second Getter
    /// @return Target Frames Per Second
    const unsigned int& TargetFPS();

    /// @brief Resources Getter
    /// @return Path to the resources folder
    const std::string& Resources();

    /// @brief Fullscreen Setter
    /// @param fullscreen 1 if fullscreen is true, 0 if false
    void Fullscreen(const bool& fullscreen);

    /// @brief Music Setter
    /// @param music 1 if music is true, 0 if false
    void Music(const bool& music);

    /// @brief Width Setter
    /// @param width The width of the window
    void Width(const int& width);

    /// @brief Height Setter
    /// @param height The height of the window
    void Height(const int& height);

    /// @brief TargetFPS Setter
    /// @param targetFPS The targetFPS
    void TargetFPS(const int& targetFPS);

private:
    /// @brief Assigns default values for appdata
    void CreateSettingsFile();

    /// @brief Reads from the settings file and sets the value of fullscreen
    void FindFullscreen();

    /// @brief Reads from the settings file and sets the value of music
    void FindMusic();

    /// @brief Reads from the settings file and sets the value of width
    void FindWidth();

    /// @brief Reads from the settings file and sets the value of height
    void FindHeight();

    /// @brief Reads from the settings file and sets the value of targetFPS
    void FindTargetFPS();

    /// @brief Reads from the settings file and sets the value of resources
    void FindResources();

    /// @brief File stream for the settings file
    std::fstream file;

    /// @brief Mutex for the file stream
    std::mutex fileGuard;

    /// @brief Fullscreen value, 1 if true, 0 if false
    bool fullscreen;

    /// @brief Mutex for fullscreen
    std::mutex fullscreenGuard;

    /// @brief Music value, 1 if true, 0 if false
    bool music;

    /// @brief Mutex for music
    std::mutex musicGuard;

    /// @brief Width of the window
    unsigned int width;

    /// @brief Mutex for width
    std::mutex widthGuard;

    /// @brief Height of the window
    unsigned int height;

    /// @brief Mutex for height
    std::mutex heightGuard;

    /// @brief Target Frames Per Second
    unsigned int targetFPS;

    /// @brief Mutex for targetFPS
    std::mutex targetFPSGuard;

    /// @brief Path to the resources folder
    std::string resources;

    /// @brief Mutex for resources
    std::mutex resourcesGuard;

    /// @brief Path to the appdata folder
    std::string appdata;

    /// @brief Path to the settings file
    std::string path;
};

#endif /* APPDATA_HPP */
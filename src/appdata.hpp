#ifndef APPDATA_HPP
#define APPDATA_HPP

#include <fstream>
#include <sstream>
#include <mutex>
#include <future>
#include <vector>

class AppData {
public:
    AppData();
    ~AppData();

    const bool& Fullscreen();
    const unsigned int& Width();
    const unsigned int& Height();
    const unsigned int& TargetFPS();
    const std::string& Resources();

    void Fullscreen(const bool&);
    void Width(const int&);
    void Height(const int&);
    void TargetFPS(const int&);

private:    
    void CreateSettingsFile();
    void FindFullscreen();
    void FindWidth();
    void FindHeight();
    void FindTargetFPS();
    void FindResources();

    std::fstream file;
    std::mutex fileGuard;

    bool fullscreen;
    std::mutex fullscreenGuard;

    unsigned int width;
    std::mutex widthGuard;

    unsigned int height;
    std::mutex heightGuard;

    unsigned int targetFPS;
    std::mutex targetFPSGuard;

    std::string resources;
    std::mutex resourcesGuard;
};

#endif /* APPDATA_HPP */
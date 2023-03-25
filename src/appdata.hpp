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

    const unsigned int& Width();
    const unsigned int& Height();
    const unsigned int& TargetFPS();
    const std::string& Resources();

    void Width(const int&);
    void Height(const int&);
    void TargetFPS(const int&);

private:
    void CreateSettingsFile();
    void FindWidth();
    void FindHeight();
    void FindTargetFPS();
    void FindResources();

    std::fstream file;
    std::mutex fileGuard;

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
#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <cstring>
#include <iostream>

AppData::AppData() {

    appdata = getenv("APPDATA");
    path = appdata + "/NEAGame/settings.txt";

    file.open(path, std::fstream::in);

    if (file.is_open()) {
        std::vector<std::future<void>> queue;

        std::fstream::in;

        queue.emplace_back(std::async(&AppData::FindFullscreen, this));
        queue.emplace_back(std::async(&AppData::FindMusic, this));
        queue.emplace_back(std::async(&AppData::FindTargetFPS, this));
        queue.emplace_back(std::async(&AppData::FindResources, this));

        queue.begin()->wait();

        if (fullscreen) {
            width = GetSystemMetrics(SM_CXSCREEN);
            height = GetSystemMetrics(SM_CYSCREEN);
        } else {
            queue.emplace_back(std::async(&AppData::FindWidth, this));
            queue.emplace_back(std::async(&AppData::FindHeight, this));
        }

        for (std::future<void>& i : queue) i.wait();

    } else {
        CreateSettingsFile();
    }
    file.close();
}

AppData::~AppData() {
    file.open(path, std::fstream::out | std::fstream::trunc);

    file << (
        "fullscreen=" + std::to_string(fullscreen) + "\n" +
        "music=" + std::to_string(music) + "\n" +
        "width=" + std::to_string(width) + "\n" +
        "height=" + std::to_string(height) + "\n" +
        "targetFPS=" + std::to_string(targetFPS) + "\n" +
        "resources=" + resources
    );

    file.close();
}

const bool& AppData::Fullscreen() {
    std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);
    return fullscreen;
}

const bool& AppData::Music() {
    std::scoped_lock<std::mutex> musicLock(musicGuard);
    return music;
}

const unsigned int& AppData::Width() {
    std::scoped_lock<std::mutex> widthLock(widthGuard);
    return width;
}

const unsigned int& AppData::Height() {
    std::scoped_lock<std::mutex> heightLock(heightGuard);
    return height;
}

const unsigned int& AppData::TargetFPS() {
    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
    return targetFPS;
}

const std::string& AppData::Resources() {
    std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);
    return resources;
}

void AppData::Fullscreen(const bool& fullscreen) {
    std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);
    std::scoped_lock<std::mutex> widthLock(widthGuard);
    std::scoped_lock<std::mutex> heightLock(heightGuard);

    this->fullscreen = fullscreen;

    if (fullscreen) {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    } else {
        std::scoped_lock<std::mutex> fileLock(fileGuard);
        FindWidth();
        FindHeight();
    }
}

void AppData::Music(const bool& music) {
    std::scoped_lock<std::mutex> musicLock(musicGuard);
    this->music = music;
}

void AppData::Width(const int& width) {
    std::scoped_lock<std::mutex> widthLock(widthGuard);    
    this->width = width;
}

void AppData::Height(const int& height) {
    std::scoped_lock<std::mutex> heightLock(heightGuard);
    this->height = height;
}

void AppData::TargetFPS(const int& targetFPS) {
    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
    this->targetFPS = targetFPS;
}

void AppData::CreateSettingsFile() {
    if (!std::filesystem::is_directory(appdata + "/NEAGame/")) {
        std::filesystem::create_directory(appdata + "/NEAGame/");
    }

    fullscreen = true;

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    targetFPS = 60;

    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";
}

void AppData::FindFullscreen() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);
    
    file.clear();
    file.seekg(0 , std::ios::beg);

    while (getline(file, line)) {
        if (line.find("fullscreen=") != std::string::npos) {
            fileLock.unlock();
            std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);
            fullscreen = std::stoi(line.substr(line.find("=")+1, line.length()));
            return;
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);

    fullscreen = true;
}

void AppData::FindMusic() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (getline(file, line)) {
        if (line.find("music=") != std::string::npos) {
            fileLock.unlock();
            std::scoped_lock<std::mutex> musicLock(musicGuard);
            music = std::stoi(line.substr(line.find("=")+1, line.length()));
            return;
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> musicLock(musicGuard);

    music = true;
}

void AppData::FindWidth() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (getline(file, line)) {
        if (line.find("width=") != std::string::npos) {
            fileLock.unlock();
            std::scoped_lock<std::mutex> widthLock(widthGuard);
            width = std::stoi(line.substr(line.find("=")+1, line.length()));
            return;
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> widthLock(widthGuard);

    width = GetSystemMetrics(SM_CXSCREEN);
}

void AppData::FindHeight() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (getline(file, line)) {
        if (line.find("height=") != std::string::npos) {
            fileLock.unlock();
            std::scoped_lock<std::mutex> heightLock(heightGuard);
            height = std::stoi(line.substr(line.find("=")+1, line.length()));
            return;
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> heightLock(heightGuard);

    height = GetSystemMetrics(SM_CYSCREEN);
}

void AppData::FindTargetFPS() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (getline(file, line)) {
        if (line.find("targetFPS=") != std::string::npos) {
            fileLock.unlock();
            std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
            targetFPS = std::stoi(line.substr(line.find("=")+1, line.length()));
            return;
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);

    targetFPS = 60;
}

void AppData::FindResources() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (getline(file, line)) {
        if (line.find("resources=") != std::string::npos) {
            fileLock.unlock();
            std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);
            resources = line.substr(line.find("=")+1, line.length());
            return;
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);

    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";
}

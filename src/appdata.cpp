#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <cstring>
#include <iostream>

AppData::AppData() {
    file.open("C:/Users/Daniel/AppData/Roaming/NEAGame/settings.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (file) {
        std::vector<std::future<void>> queue;

        queue.emplace_back(std::async(&AppData::FindFullscreen, this));
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
}

AppData::~AppData() {
    std::scoped_lock<std::mutex> fileLock(fileGuard);
    file.close();
}

const unsigned int& AppData::Width() {
    std::scoped_lock<std::mutex> widthLock(widthGuard);
    return width;
}

const unsigned int& AppData::Height() {
    std::scoped_lock<std::mutex> heightLock(heightGuard);
    return height;
}

const bool& AppData::Fullscreen() {
    std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);
    return fullscreen;
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
    std::unique_lock<std::mutex> fullscreenLock(fullscreenGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);

    this->fullscreen = fullscreen;

    std::stringstream _file;

    fileLock.lock();
    _file << file.rdbuf();
    fileLock.unlock();

    std::string text = _file.str();
    std::string search = "fullscreen=";
    std::string replace = "fullscreen=" + fullscreen;
    size_t index = text.find(search);
    text.replace(index, std::string(search).length(), replace);

    fileLock.lock();    
    file << text;
}

void AppData::Width(const int& width) {
    std::unique_lock<std::mutex> widthLock(widthGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);
    
    this->width = width;

    std::stringstream _file;

    fileLock.lock();
    _file << file.rdbuf();
    fileLock.unlock();

    std::string text = _file.str();
    std::string search = "width=";
    std::string replace = "width=" + width;
    size_t index = text.find(search);
    text.replace(index, std::string(search).length(), replace);

    fileLock.lock();
    file << text;
}

void AppData::Height(const int& height) {
    std::unique_lock<std::mutex> heightLock(heightGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);

    this->height = height;

    std::stringstream _file;

    fileLock.lock();
    _file << file.rdbuf();
    fileLock.unlock();

    std::string text = _file.str();
    std::string search = "height=";
    std::string replace = "height=" + height;
    size_t index = text.find(search);
    text.replace(index, std::string(search).length(), replace);

    fileLock.lock();    
    file << text;
}


void AppData::TargetFPS(const int& targetFPS) {
    std::unique_lock<std::mutex> targetFPSLock(targetFPSGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);

    this->targetFPS = targetFPS;

    std::stringstream _file;

    fileLock.lock();
    _file << file.rdbuf();
    fileLock.unlock();

    std::string text = _file.str();
    std::string search = "targetFPS=";
    std::string replace = "targetFPS=" + targetFPS;
    size_t index = text.find(search);
    text.replace(index, std::string(search).length(), replace);

    fileLock.lock();    
    file << text;
}

void AppData::CreateSettingsFile() {
    if (!std::filesystem::is_directory("C:/Users/Daniel/AppData/Roaming/NEAGame/")) {
        std::filesystem::create_directory("C:/Users/Daniel/AppData/Roaming/NEAGame/");
    }

    file.open("C:/Users/Daniel/AppData/Roaming/NEAGame/settings.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

    fullscreen = true;

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    targetFPS = 60;

    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";

    file << (
        "fullscreen=" + std::to_string(fullscreen) +
        "\nwidth=" + std::to_string(width) +
        "\nheight=" + std::to_string(height) +
        "\ntargetFPS=" + std::to_string(targetFPS) +
        "\nresources=" + resources + "\n");
}

void AppData::FindFullscreen() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);
    
    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("fullscreen=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);

            fullscreen = true;

            file.clear();

            file << "fullscreen=" + std::to_string(fullscreen) + "\n";
            return;
        } else {
            std::getline(file, line);
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> fullscreenLock(fullscreenGuard);
    fullscreen = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindWidth() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("width=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> widthLock(widthGuard);

            width = GetSystemMetrics(SM_CXSCREEN);

            file.clear();
            
            file << "width=" + std::to_string(width) + "\n";
            return;
        } else {
            std::getline(file, line);
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> widthLock(widthGuard);
    width = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindHeight() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("height=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> heightLock(heightGuard);

            height = GetSystemMetrics(SM_CYSCREEN);

            file.clear();

            file << "height=" + std::to_string(height) + "\n";
            return;
        } else {
            std::getline(file, line);
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> heightLock(heightGuard);
    height = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindTargetFPS() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("targetFPS=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);

            targetFPS = 60;

            file.clear();

            file << "targetFPS=" + std::to_string(targetFPS) + "\n";
            return;
        } else {
            std::getline(file, line);
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
    targetFPS = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindResources() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("resources=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);

            if (std::filesystem::is_directory("resources")) resources = "resources";
            else if (std::filesystem::is_directory("../resources")) resources = "../resources";
            else throw "Unable to find resources directory.";

            file.clear();

            file << "resources=" + resources + "\n";
            return;
        } else {
            std::getline(file, line);
        }
    }

    fileLock.unlock();
    
    std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);
    resources = line.substr(line.find("=")+1, line.length());
}
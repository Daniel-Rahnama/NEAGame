#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <cstring>
#include <iostream>

AppData::AppData() {
    if (std::filesystem::is_directory("C:/Users/Daniel/AppData/Roaming/NEAGame/")) {
        file.open("C:/Users/Daniel/AppData/Roaming/NEAGame/settings.txt");

        std::vector<std::future<void>> queue;

        queue.emplace_back(std::async(&AppData::FindWidth, this));
        queue.emplace_back(std::async(&AppData::FindHeight, this));
        queue.emplace_back(std::async(&AppData::FindTargetFPS, this));
        queue.emplace_back(std::async(&AppData::FindResources, this));

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

const unsigned int& AppData::TargetFPS() {
    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
    return targetFPS;
}

const std::string& AppData::Resources() {
    std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);
    return resources;
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
    std::filesystem::create_directory("C:/Users/Daniel/AppData/Roaming/NEAGame/");
    file.open("C:/Users/Daniel/AppData/Roaming/NEAGame/settings.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    targetFPS = 60;

    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";

    file << ("width=" + std::to_string(width) +
        "\nheight=" + std::to_string(height) +
        "\ntargetFPS=" + std::to_string(targetFPS) +
        "\nresources=" + resources);
}

void AppData::FindWidth() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    while (line.find("width=") == std::string::npos) {
        std::getline(file, line);
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> widthLock(widthGuard);
    width = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindHeight() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    while (line.find("height=") == std::string::npos) {
        std::getline(file, line);
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> heightLock(heightGuard);
    height = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindTargetFPS() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    while (line.find("targetFPS=") == std::string::npos) {
        std::getline(file, line);
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
    targetFPS = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindResources() {
    std::string line = "";

    std::unique_lock<std::mutex> fileLock(fileGuard);

    while (line.find("resources=") == std::string::npos) {
        std::getline(file, line);
    }

    fileLock.unlock();
    
    std::scoped_lock<std::mutex> resourcesLock(resourcesGuard);
    resources = line.substr(line.find("=")+1, line.length()+1);
}
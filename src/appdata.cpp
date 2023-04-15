#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <cstring>
#include <iostream>

AppData::AppData() {

    appdata = getenv("APPDATA");
    path = appdata + "/NEAGame/settings.txt";

    file.open(path, std::fstream::app);

    if (file) {
        std::vector<std::future<void>> queue;

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
}

AppData::~AppData() {
    std::scoped_lock<std::mutex> fileLock(fileGuard);
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
    std::unique_lock<std::mutex> fullscreenLock(fullscreenGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);

    this->fullscreen = fullscreen;

    if (fullscreen) {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
    } else {
        FindWidth();
        FindHeight();
    }

    std::stringstream _file;

    file.clear();
    file.seekg(0, std::ios::beg);

    _file << file.rdbuf();

    std::string text = _file.str();

    std::string search = "fullscreen=";
    std::string replace = "fullscreen=" + fullscreen;

    size_t index = text.find(search);

    file.seekg(0, std::ios::beg);

    std::string line;

    while (line.find(search) == std::string::npos) {
        std::getline(file, line);
    }

    file.seekg(0, std::ios::beg);

    text.replace(index, line.length(), replace);

    file.close();
    file.open(path, std::fstream::trunc);

    file << text;

    file.close();
    file.open(path, std::fstream::app);
}

void AppData::Music(const bool& music) {
    std::unique_lock<std::mutex> musicLock(musicGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);

    this->music = music;

    std::stringstream _file;

    file.clear();
    file.seekg(0, std::ios::beg);

    _file << file.rdbuf();

    std::string text = _file.str();

    std::string search = "music=";
    std::string replace = "music=" + std::to_string(music);

    size_t index = text.find(search);

    file.seekg(0, std::ios::beg);

    std::string line;

    while (line.find(search) == std::string::npos) {
        std::getline(file, line);
    }

    file.seekg(0, std::ios::beg);

    text.replace(index, line.length(), replace);

    file.close();
    file.open(path, std::fstream::trunc);

    file << text;
    
    file.close();
    file.open(path, std::fstream::app);
}

void AppData::Width(const int& width) {
    std::unique_lock<std::mutex> widthLock(widthGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);
    
    this->width = width;

    std::stringstream _file;

    file.clear();
    file.seekg(0, std::ios::beg);

    _file << file.rdbuf();

    std::string text = _file.str();

    std::string search = "width=";
    std::string replace = "width=" + std::to_string(width);

    size_t index = text.find(search);

    file.seekg(0, std::ios::beg);

    std::string line;

    while (line.find(search) == std::string::npos) {
        std::getline(file, line);
    }

    file.seekg(0, std::ios::beg);

    text.replace(index, line.length(), replace);

    file.close();
    file.open(path, std::fstream::trunc);

    file << text;

    file.close();
    file.open(path, std::fstream::app);
}

void AppData::Height(const int& height) {
    std::unique_lock<std::mutex> heightLock(heightGuard);
    std::unique_lock<std::mutex> fileLock(fileGuard, std::defer_lock);

    this->height = height;

    std::stringstream _file;

    file.clear();
    file.seekg(0, std::ios::beg);

    _file << file.rdbuf();

    std::string text = _file.str();

    std::string search = "height=";
    std::string replace = "height=" + std::to_string(height);

    size_t index = text.find(search);

    file.seekg(0, std::ios::beg);

    std::string line;

    while (line.find(search) == std::string::npos) {
        std::getline(file, line);
    }

    file.seekg(0, std::ios::beg);

    text.replace(index, line.length(), replace);

    file.close();
    file.open(path, std::fstream::trunc);

    file << text;

    file.close();
    file.open(path, std::fstream::app);
}

void AppData::TargetFPS(const int& targetFPS) {
    std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);
    std::scoped_lock<std::mutex> fileLock(fileGuard);

    this->targetFPS = targetFPS;

    std::stringstream _file;

    file.close();
    file.open(path, std::fstream::app);

    file.clear();
    file.seekg(0, std::ios::beg);

    _file << file.rdbuf();

    std::string text = _file.str();

    std::string search = "targetFPS=";
    std::string replace = "targetFPS=" + std::to_string(targetFPS);

    size_t index = text.find(search);

    file.clear();
    file.seekg(0, std::ios::beg);

    std::string line;

    std::cout << "reached 1";

    while (line.find(search) == std::string::npos) {
        std::getline(file, line);
    }

    std::cout << "reached 2";

    file.seekg(0, std::ios::beg);

    text.replace(index, line.length(), replace);

    file.close();
    file.open(path, std::fstream::trunc);

    file << text;

    file.close();
    file.open(path, std::fstream::app);
}

void AppData::CreateSettingsFile() {
    if (!std::filesystem::is_directory(appdata + "/NEAGame/")) {
        std::filesystem::create_directory(appdata + "/NEAGame/");
    }

    file.close();
    file.open(path, std::fstream::trunc);

    fullscreen = true;

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    targetFPS = 60;

    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";

    file << (
        "fullscreen=" + std::to_string(fullscreen) +
        "\nmusic=" + std::to_string(music) +
        "\nwidth=" + std::to_string(width) +
        "\nheight=" + std::to_string(height) +
        "\ntargetFPS=" + std::to_string(targetFPS) +
        "\nresources=" + resources + "\n");

    file.close();
    file.open(path,  std::fstream::app);
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
            file.seekg(0 , std::ios::beg);

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

void AppData::FindMusic() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("music=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> musicLock(musicGuard);

            music = true;

            file.clear();
            file.seekg(0 , std::ios::beg);

            file << "music=" + std::to_string(music) + "\n";
            return;
        } else {
            std::getline(file, line);
        }
    }

    fileLock.unlock();

    std::scoped_lock<std::mutex> musicLock(musicGuard);
    music = std::stoi(line.substr(line.find("=")+1, line.length()));
}

void AppData::FindWidth() {
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("width=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> widthLock(widthGuard);

            width = GetSystemMetrics(SM_CXSCREEN);

            file.clear();
            file.seekg(0 , std::ios::beg);

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
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("height=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> heightLock(heightGuard);

            height = GetSystemMetrics(SM_CYSCREEN);

            file.clear();
            file.seekg(0 , std::ios::beg);

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
    std::string line;

    std::unique_lock<std::mutex> fileLock(fileGuard);

    file.clear();
    file.seekg(0 , std::ios::beg);

    while (line.find("targetFPS=") == std::string::npos) {
        if (file.eof()) {
            std::scoped_lock<std::mutex> targetFPSLock(targetFPSGuard);

            targetFPS = 60;

            file.clear();
            file.seekg(0 , std::ios::beg);

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
    std::string line;

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
            file.seekg(0 , std::ios::beg);

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
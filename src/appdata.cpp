#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <cstring>
#include <iostream>

AppData::AppData(const std::string& playerUsername, const std::string& playerSpritesheet) 
    : playerUsername(playerUsername), playerSpritesheet(playerSpritesheet) {


    appdata = (std::string)getenv("APPDATA") + "/NEAGame";

    settingsFile.open(appdata + "/settings.json", std::fstream::in);

    if (settingsFile.is_open()) {
        settingsFile >> settingsJSON;
        settingsFile.close();

        fullscreen = settingsJSON["fullscreen"].asBool();
        
        if (fullscreen) {
            width = GetSystemMetrics(SM_CXSCREEN);
            height = GetSystemMetrics(SM_CYSCREEN);
        } else {
            width = settingsJSON["width"].asInt();
            height = settingsJSON["height"].asInt();
        }

        music = settingsJSON["music"].asBool();
        targetFPS = settingsJSON["targetFPS"].asInt();
        resources = settingsJSON["resources"].asString();
    } else {
        settingsFile.close();

        fullscreen = true;
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        music = true;
        targetFPS = 60;

        if (std::filesystem::is_directory("resources")) resources = std::filesystem::absolute("resources").string();
        else if (std::filesystem::is_directory("..\\resources")) resources = std::filesystem::absolute("..\\resources").string();
        else throw "Unable to find resources directory.";

        settingsJSON["fullscreen"] = fullscreen;
        settingsJSON["width"] = width;
        settingsJSON["height"] = height;
        settingsJSON["music"] = music;
        settingsJSON["targetFPS"] = targetFPS;
        settingsJSON["resources"] = resources;

        settingsFile.open(appdata + "/settings.json", std::fstream::trunc | std::fstream::out);
        settingsFile << settingsJSON;
        settingsFile.close();
    }    
    
    if (playerUsername == "") this->playerUsername = "Default User";
    if (playerSpritesheet == "") this->playerSpritesheet = resources + "/sprites/c1.png";
}

const std::string& AppData::Path() const {
    return appdata;
}

const bool& AppData::Fullscreen() const {
    return fullscreen;
}

const uint16_t& AppData::Width() const {
    return width;
}

const uint16_t& AppData::Height() const {
    return height;
}

const bool& AppData::Music() const {
    return music;
}

const uint8_t& AppData::TargetFPS() const {
    return targetFPS;
}

const std::string& AppData::Resources() const {
    return resources;
}

const std::string& AppData::PlayerUsername() const {
    return playerUsername;
}

const std::string& AppData::PlayerSpritesheet() const {
    return playerSpritesheet;
}

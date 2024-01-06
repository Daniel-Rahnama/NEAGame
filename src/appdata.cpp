#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <cstring>

AppData::AppData(const std::string& playerUsername, const std::string& playerSpritesheet, const std::string& mobSpritesheet, const std::string& playerStatsAddress, const std::string& mobStatsAddress, const std::string& difficultyLevel, const std::string& gameMissionAddress) 
    : playerUsername(playerUsername), playerSpritesheet(playerSpritesheet), mobSpritesheet(mobSpritesheet), difficultyLevel(difficultyLevel), gameMissionAddress(gameMissionAddress) {

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

        std::filesystem::create_directory(appdata);

        settingsFile.open(appdata + "/settings.json", std::fstream::trunc | std::fstream::out);
        settingsFile << settingsJSON;
        settingsFile.close();
    }    
    
    if (playerUsername == "") this->playerUsername = "Default User";
    if (playerSpritesheet == "") this->playerSpritesheet = resources + "/sprites/player.png";
    if (mobSpritesheet == "") this->mobSpritesheet = resources + "/sprites/mob.png";

    if (playerStatsAddress != "") {
        std::fstream playerStatsFile(playerStatsAddress, std::fstream::in);
        Json::Value playerStatsJson;
        playerStatsFile >> playerStatsJson;
        playerStatsFile.close();

        playerStats.speed = playerStatsJson["speed"].asInt();
        playerStats.damage = playerStatsJson["damage"].asInt();
        playerStats.delay = playerStatsJson["delay"].asInt();
        playerStats.fatigue = playerStatsJson["fatigue"].asInt();
        playerStats.regen = playerStatsJson["regen"].asDouble();
    } else {
        playerStats.speed = 4;
        playerStats.damage = 2;
        playerStats.delay = 10;
        playerStats.fatigue = 10;
        playerStats.regen = 2;
    }
    
    if (mobStatsAddress != "") {
        std::fstream mobStatsFile(mobStatsAddress, std::fstream::in);
        Json::Value mobStatsJson;
        mobStatsFile >> mobStatsJson;
        mobStatsFile.close();

        mobStats.speed = mobStatsJson["speed"].asInt();
        mobStats.damage = mobStatsJson["damage"].asInt();
        mobStats.delay = mobStatsJson["delay"].asInt();
        mobStats.fatigue = mobStatsJson["fatigue"].asInt();
        mobStats.regen = mobStatsJson["regen"].asDouble();
    } else {
        mobStats.speed = 3;
        mobStats.damage = 1;
        mobStats.delay = 15;
        mobStats.fatigue = 15;
        mobStats.regen = 1 ;
    }
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

const std::string& AppData::MobSpritesheet() const {
    return mobSpritesheet;
}

const Stats& AppData::PlayerStats() const {
    return playerStats;
}

const Stats& AppData::MobStats() const {
    return mobStats;
}

const std::string& AppData::DifficultyLevel() const {
    return difficultyLevel;
}

const std::string &AppData::GameMissionAddress() const {
    return gameMissionAddress;
}

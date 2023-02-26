#include "appdata.hpp"

#include <filesystem>
#include <Windows.h>
#include <string>

AppData::AppData() {
    if (std::filesystem::is_directory("%appdata%/NEAGame/")) { 
        file.open("%appdata%/NEAGame/settings.txt");
        width = FindWidth();
        height = FindHeight();
        resources = FindResources();
    }
    else {
        CreateSettingsFile();
    }
}

AppData::~AppData() {
    file.close();
}

const char* AppData::Resources() {
    return resources;
}

int AppData::Width() {
    return width;
}

void AppData::Width(int width) {
    this->width = width;
    std::stringstream _file << file.rdbuf();
    std::string text = _file.str();
    std::string search = "width=";
    std::string replace = "width=" + width;
    size_t index = text.find(search);
    text.replace(index, string(search).length(), replace);
    file << text;
}

int AppData::Height() {
    return height;
}

void AppData::Height(int height) {
    this->height = height;
    std::stringstream _file << file.rdbuf();
    std::string text = _file.str();
    std::string search = "height=";
    std::string replace = "height=" + height;
    size_t index = text.find(search);
    text.replace(index, string(search).length(), replace);
    file << text;
}

void AppData::CreateSettingsFile() {
    std::filesystem::create_directory("%appdata%/NEAGame/");
    file = std::fstream("%appdata%/NEAGame/settings.txt");

    width = GetSystemMetrics(SM_CXSCREEN);
    height = GetSystemMetrics(SM_CYSCREEN);

    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";

    file.write("width=" + width +
        "\nheight=" + height +
        "\nresources=" + resources);
}

unsigned int AppData::FindWidth() {
    std::string line = "";
    while (line.find("width=") != std::string::npos) {
        std::getline(file, line);
    }
    return line.substr(1, line.find("="));
}

unsigned int AppData::FindHeight() {
    std::string line = "";
    while (line.find("height=") != std::string::npos) {
        std::getline(file, line);
    }
    return line.substr(1, line.find("="));
}

const char* AppData::FindResources() {
    std::string line = "";
    while (line.find("resources=") != std::string::npos) {
        std::getline(file, line);
    }
    return line.substr(1, line.find("="));
}
#include "main.hpp"

#include "controller.hpp"
#include "renderer.hpp"
#include "game.hpp"

#include <windows.h>
#include <filesystem>

int main(int argc, char *argv[]) {
    try {
        findResources();
        Renderer renderer(width, height, resources);
        Controller controller;
        Game(width, height, resources).Run(target_frame_duration, renderer, controller);
    } catch (const char* exception)  {
        MessageBoxA(NULL, exception, "Exception", MB_ICONERROR | MB_OK);
    }
    return 0;
}

void findResources() {
    if (std::filesystem::is_directory("resources")) resources = "resources";
    else if (std::filesystem::is_directory("../resources")) resources = "../resources";
    else throw "Unable to find resources directory.";
}
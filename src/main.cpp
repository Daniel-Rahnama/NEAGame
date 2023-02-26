#include "main.hpp"

#include "appdata.hpp"
#include "controller.hpp"
#include "renderer.hpp"
#include "game.hpp"

#include <windows.h>

int main(int argc, char *argv[]) {
    try {
        AppData appdata;
        Renderer renderer(appdata.width(), appdata.height(), appdata.resources());
        Controller controller;
        Game(appdata.width(), appdata.height(), appdata.resources()).Run(target_frame_duration, renderer, controller);
    } catch (const char* exception)  {
        MessageBoxA(NULL, exception, "Exception", MB_ICONERROR | MB_OK);
    }
    return 0;
}